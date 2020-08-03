#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pma.h"

#define LAST_INDEX(ARR) (sizeof(ARR) / sizeof(ARR[0]) - 1)

#define PMA_BYTES_NUMBER (256)

#define PRE_BUF_LENGTH (16)
#define POST_BUF_LENGTH (16)


uint8_t inp_buf_pool[PRE_BUF_LENGTH + PMA_BYTES_NUMBER + POST_BUF_LENGTH] = {0};
uint8_t* const inp_buf = &inp_buf_pool[PRE_BUF_LENGTH];
uint8_t expected[PMA_BYTES_NUMBER];

#ifdef STM32F107xC
void pma_pool_init(void)
{
	return;
}
#else
// 2 data bytes per 4 address bytes   ||     // 2 data bytes per 2 address bytes
// 0x40006000   pma[0]                ||     // 0x40006000   pma[0]
// 0x40006001   pma[1]                ||     // 0x40006001   pma[1]
// 0x40006002    ---                  ||     // 0x40006002   pma[2]
// 0x40006003    ---                  ||     // 0x40006003   pma[3]
// 0x40006004   pma[2]                ||     // 0x40006004   pma[4]
// 0x40006005   pma[3]                ||     // 0x40006005   pma[5]
// 0x40006006    ---                  ||     // 0x40006006   pma[6]
// 0x40006007    ---                  ||     // 0x40006007   pma[7]
//  ...                               ||     //  ...
#define PMA_MULT (sizeof(pma_uint16_t) / sizeof(uint16_t))
#define PMA_SIZE (PMA_BYTES_NUMBER * PMA_MULT)

uint8_t pool[PMA_SIZE];
void pma_pool_init(void)
{
	for(size_t i=0; i < sizeof(pool); i++) {
		pool[i] = (uint8_t)i;
	}
}
#endif

//           pool: 00 01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F
//expected[] ADDR: 00 01        04 05        08 09        0C 0D
//            PMA: 00 01        02 03        04 05        06 07

void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
	pma_pool_init();

	for(size_t i=0; i < sizeof(expected); i+=2)
	{
		expected[i] = (uint8_t)(i*2);
		expected[i+1] = (uint8_t)(i*2 + 1);
	}
	/*for(size_t i=0; i < sizeof(expected); i++)
	{
		printf("%02X, ", expected[i]);
	}
	printf("\n");*/
}

void tearDown(void)
{
}


/** \brief Test of correct generation of ethalon sequence
 *
 */
void test_expected_array(void)
{
	uint8_t expected_arr[] = { 0x00, 0x01, 0x04, 0x05, 0x08, 0x09, 0x0C, 0x0D};
	TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_arr, expected, sizeof(expected_arr));
}

/** \brief Test of reading 8-bit integer from aligned and unaligned to 16-bit PMA address
 *
 */
void test_u8(void)
{
	TEST_ASSERT_EQUAL_HEX8( 0, read_pma_u8(0));
	TEST_ASSERT_EQUAL_HEX8( 1, read_pma_u8(1));
	TEST_ASSERT_EQUAL_HEX8( 4, read_pma_u8(2));
	TEST_ASSERT_EQUAL_HEX8( 5, read_pma_u8(3));
}

/** \brief Test of reading 16-bit integer from aligned and unaligned to 16-bit PMA address
 *
 */
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

/** \brief Test of byte-by-byte reading method
 *
 */
void test_read_from_pma_slow(void)
{
	for (size_t sz = 1; sz < sizeof(expected); sz++)
	{
		memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
		read_from_pma_slow(0, inp_buf, sz);
		TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, inp_buf, sz);
		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf + sz, PMA_BYTES_NUMBER - sz);

		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
	}
}

/** \brief Test of slow reading ZERO bytes from ANY PMA address
 *
 */
void test_read_from_pma_zero_slow(void)
{
	size_t sz = 0;
	memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
	read_from_pma_slow(0, inp_buf, sz);
	TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf, PMA_BYTES_NUMBER);

	TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
	TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
}

/** \brief Test of reading ZERO bytes from ANY PMA address
 *
 */
void test_read_from_pma_zero(void)
{
	for (size_t shift = 0; shift < PMA_BYTES_NUMBER; shift += 1)
	{
		size_t sz = 0;
		memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
		read_pma(shift, inp_buf, sz);
		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf, PMA_BYTES_NUMBER);

		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
		TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
	}
}

// PMA: 00 01        02 03        04 05        06 07
//                                 ^--shift=4
void test_read_from_pma_shifted(void)
{
	for (size_t shift = 0; shift < PMA_BYTES_NUMBER; shift++)
	{
		//printf("shift=%ld\n", shift);
		for (size_t sz = 1; sz < (PMA_BYTES_NUMBER - shift); sz++)
		{
			//printf("\tsize=%ld\n", sz);
			memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
			read_from_pma_slow(shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf + sz, PMA_BYTES_NUMBER - sz);

			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
		}
	}
}

/** \brief Test of reading EVEN number of bytes from ALIGNED to 16bit PMA address
 *  ANY* - less than size of PMA
 */
void test_read_from_pma_aligned_even_bytes_number(void)
{
	for (size_t shift = 0; shift < PMA_BYTES_NUMBER; shift += 2)
	{
		for (size_t sz = 2; sz < (PMA_BYTES_NUMBER - shift); sz += 2)
		{
			memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
			read_pma_aligned(shift, inp_buf, sz);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf + sz, PMA_BYTES_NUMBER - sz);

			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
		}
	}
}


/** \brief Test of reading ANY* number of bytes from ALIGNED to 16bit PMA address
 *  ANY* - less than size of PMA
 */
void test_read_from_pma_aligned(void)
{
	for (size_t shift = 0; shift < PMA_BYTES_NUMBER; shift += 2)
	{
		//printf("shift = %ld\n", shift);
		for (size_t sz = 1; sz < (PMA_BYTES_NUMBER - shift); sz += 1)
		{
			//printf("\tsize = %ld\n", sz);
			memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
			read_pma(shift, inp_buf, sz);
			//char msg[128];
			//snprintf(msg, sizeof(msg), "shift=%ld, size=%ld", shift, sz);
			//TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected + shift, inp_buf, sz, msg);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf + sz, PMA_BYTES_NUMBER - sz);

			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
		}
	}
}

/** \brief Test of reading ANY* number of bytes from ANY PMA address
 *  ANY* - less than size of PMA
 */
void test_read_from_pma_any(void)
{
	for (size_t shift = 0; shift < PMA_BYTES_NUMBER; shift += 1)
	{
		//printf("shift = %ld\n", shift);
		for (size_t sz = 1; sz < (PMA_BYTES_NUMBER - shift); sz += 1)
		{
			//printf("\tsize = %ld\n", sz);
			memset(inp_buf_pool, 0, sizeof(inp_buf_pool));
			read_pma(shift, inp_buf, sz);
			//char msg[128];
			//snprintf(msg, sizeof(msg), "shift=%ld, size=%ld\n", shift, sz);
			//TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected + shift, inp_buf, sz, msg);
			TEST_ASSERT_EQUAL_HEX8_ARRAY(expected + shift, inp_buf, sz);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf + sz, PMA_BYTES_NUMBER - sz);

			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool, PRE_BUF_LENGTH);
			TEST_ASSERT_EACH_EQUAL_HEX8(0x00, inp_buf_pool + PRE_BUF_LENGTH + PMA_BYTES_NUMBER, POST_BUF_LENGTH);
		}
	}
}
