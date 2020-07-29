#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "pma.h"
#include "_syscalls.h"
#include "_sbrk.h"
#include <stdio.h>

#define LAST_INDEX(ARR) (sizeof(ARR) / sizeof(ARR[0]) - 1)
uint8_t pool[2048];
uint8_t expected[sizeof(pool) / 2];

//           pool: 00 01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F
//expected[] ADDR: 00 01        04 05        08 09        0C 0D
//            PMA: 00 01        02 03        04 05        06 07

void setUp(void)
{
	for(size_t i=0; i < sizeof(pool); i++) {
		pool[i] = (uint8_t)i;
	}

	for(size_t i=0; i < sizeof(expected); i+=2) {
		expected[i] = (uint8_t)(i*2);
		expected[i+1] = (uint8_t)(i*2 + 1);
	}
	/*for(size_t i=0; i < sizeof(expected); i++) {
		printf("%02X, ", expected[i]);
	}
	printf("\n");*/
}

void tearDown(void)
{
}

void test_expected_array(void)
{
	uint8_t expected_arr[] = { 0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0C, 0x0D};
	TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_arr, expected, sizeof(expected_arr));
}

void test_u8(void)
{
	TEST_ASSERT_EQUAL_HEX8( 0, read_pma_u8(0));
	TEST_ASSERT_EQUAL_HEX8( 1, read_pma_u8(1));
	TEST_ASSERT_EQUAL_HEX8( 4, read_pma_u8(2));
	TEST_ASSERT_EQUAL_HEX8( 5, read_pma_u8(3));
}

void test_u16(void)
{
	TEST_ASSERT_EQUAL_HEX16(0x0100, read_pma_u16(0));
	TEST_ASSERT_EQUAL_HEX16(0x0401, read_pma_u16(1));
	TEST_ASSERT_EQUAL_HEX16(0x0504, read_pma_u16(2));
	TEST_ASSERT_EQUAL_HEX16(0x0805, read_pma_u16(3));
	TEST_ASSERT_EQUAL_HEX16(0x0908, read_pma_u16(4));
	TEST_ASSERT_EQUAL_HEX16(0x0C09, read_pma_u16(5));
	TEST_ASSERT_EQUAL_HEX16(0x0D0C, read_pma_u16(6));
}

void test_read_from_pma(void)
{
	uint8_t inp_buf[2048] = {0};

	for (size_t n = 1; n < sizeof(expected); n++)
	{
		memset(inp_buf, 0, sizeof(inp_buf));
		read_from_pma_slow(0, inp_buf, n);
		TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, inp_buf, n);
	}

	for (size_t shift = 0; shift < sizeof(expected)/2; shift++)
	{
		for (size_t sz = 1; sz < sizeof(expected)/2; sz++)
		{
			//printf("testing shift=%ld, size=%ld\n", shift, sz);
			memset(inp_buf, 0, sizeof(inp_buf));
			read_from_pma_slow(shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
		}
	}
}
