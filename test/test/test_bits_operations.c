#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "bits_operation.h"



void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
}

void tearDown(void)
{
}


uint8_t w_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  0,  0,  1,  1,  1,      // normal-write bits
};

uint8_t w_set_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  1,  0,  1,  1,  1,      // normal-write bits
};

uint8_t w_clear_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  1,  0,  0,  1,  0,      // normal-write bits
};

uint8_t w0_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  0,  0,  0,  1,  1,      // write-0 bits
};


uint8_t t_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  1,  0,  1,  1,  0,      // toggleable bits
};

uint8_t t_set_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  1,  0,  1,  1,  1,      // set toggleable bits
};

uint8_t t_clear_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  1,  0,  0,  1,  0,      // clear toggleable bits
};

void test_tables(void)
{
	// v -- bit's value to be written
	// r -- register bit's value
	// m -- mask, 1 - write is permitted, 0 - write is forbidden
	for (size_t v = 0; v < 2; v++)
	{
		for (size_t r = 0; r < 2; r++)
		{
			for (size_t m = 0; m < 2; m++)
			{
				uint8_t index = (v<<2) | (r<<1) | (m);
				
				uint8_t w1  = w_tbl[index];
				uint8_t w_set1= w_set_tbl[index];
				uint8_t w_clear1= w_clear_tbl[index];
				uint8_t w01 = w0_tbl[index];
				uint8_t rt1 = t_tbl[index];
				uint8_t rts1 = t_set_tbl[index];
				uint8_t rtc1 = t_clear_tbl[index];
				
				uint8_t w2  = r;
				uint8_t w_set2= r;
				uint8_t w_clear2= r;
				uint8_t w02 = r;
				uint8_t rt2 = r;
				uint8_t rts2 = r;
				uint8_t rtc2 = r;
				if (m == 1) {
					 // write writable
					w2 = v;
					
					 // set writable
					if (v == 1) {
						w_set2 = 1;
					}

					 // clear writable
					if (v == 1) {
						w_clear2 = 0;
					}

					 // write-0
					if (v == 0) {
						w02 = 0;
					}
					
					// toggle;
					rt2 = r ^ v;

					// set toggleable;
					rts2 = (v == 1) ? 1 : rts2;

					// clear toggleable;
					rtc2 = (v == 1) ? 0 : rtc2;
				}
				TEST_ASSERT_EQUAL_UINT8(w1 , w2);
				TEST_ASSERT_EQUAL_UINT8(w_set1 , w_set2);
				TEST_ASSERT_EQUAL_UINT8(w_clear1 , w_clear2);
				TEST_ASSERT_EQUAL_UINT8(w01, w02);
				TEST_ASSERT_EQUAL_UINT8(rt1, rt2);				
				TEST_ASSERT_EQUAL_UINT8(rts1, rts2);				
				TEST_ASSERT_EQUAL_UINT8(rtc1, rtc2);				
			}
		}
	}
}


void test_w(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_write_bits(val, reg, w_mask);

	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w_mask >>= 1;
	}
}

void test_w_set(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_set_bits(val, reg, w_mask);

	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w_set_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w_mask >>= 1;
	}
}

void test_w_clear(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_clear_bits(val, reg, w_mask);

	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w_clear_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w_mask >>= 1;
	}
}

void test_w0(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t w0_mask = 0b01010101;

	uint32_t result = w0_write_bits(val, reg, w0_mask);

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w0_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w0_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w0_mask >>= 1;
	}
}


void test_t(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;

	uint32_t result = t_write_bits(val, reg, t_mask);

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}

void test_t_set_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;

	uint32_t result = t_set_bits(val, reg, t_mask);
	result = reg  ^ result; // Emulating hardware-XOR-ing

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t_set_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}

void test_t_clear_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;

	uint32_t result = t_clear_bits(val, reg, t_mask);
	result = reg  ^ result; // Emulating hardware-XOR-ing

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t_clear_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}
