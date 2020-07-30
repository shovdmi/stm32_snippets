#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "pma.h"
//#include "_syscalls.h"
//#include "_sbrk.h"
#include <stdio.h>

#define LAST_INDEX(ARR) (sizeof(ARR) / sizeof(ARR[0]) - 1)
#define PMA_SIZE (512)

uint8_t pool[PMA_SIZE * 4];
uint8_t expected[PMA_SIZE * 2];
uint8_t inp_buf[PMA_SIZE * 4] = {0};
const uint8_t zero_buf[PMA_SIZE * 4] = {0};

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
	for (size_t sz = 1; sz < sizeof(expected); sz++)
	{
		memset(inp_buf, 0, sizeof(inp_buf));
		read_from_pma_slow(0, inp_buf, sz);
		TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, inp_buf, sz);
	}
}

// PMA: 00 01        02 03        04 05        06 07
//                                 ^--shift=4
void test_read_from_pma_shifted(void)
{
	for (size_t shift = 0; shift < PMA_SIZE; shift++)
	{
		//printf("shift=%ld\n", shift);
		for (size_t sz = 1; sz < PMA_SIZE; sz++)
		{
			//printf("\tsize=%ld\n", sz);
			memset(inp_buf, 0, sizeof(inp_buf));
			read_from_pma_slow(shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(zero_buf, inp_buf + sz, sizeof(inp_buf) - sz);
		}
	}
}


void test_read_from_pma_aligned(void)
{
	for (size_t shift = 0; shift < PMA_SIZE; shift += 2)
	{
		//printf("shift = %ld\n", shift);
		for (size_t sz = 2; sz < PMA_SIZE; sz+=2)
		{
			//printf("\tsize = %ld\n", sz);
			memset(inp_buf, 0, sizeof(inp_buf));
			read_pma_aligned(shift, inp_buf, sz);
			//char msg[128];
			//snprintf(msg, sizeof(msg), "shift=%ld, size=%ld", shift, sz);
			//TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected + shift, inp_buf, sz, msg);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(zero_buf, inp_buf + sz, sizeof(inp_buf) - sz);
		}
	}
}
