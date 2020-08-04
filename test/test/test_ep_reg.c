#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
}

void tearDown(void)
{
}


uint8_t w[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  0,  0,  1,  1,  1,      // normal-write bits
};

uint8_t w0[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  0,  0,  0,  1,  1,      // write-0 bits
};


uint8_t t[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  1,  0,  1,  1,  0,      // toggleble bits
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
				
				uint8_t w1  = w[index];
				uint8_t w01 = w0[index];
				uint8_t rt1 = t[index];
				
				uint8_t w2  = r;
				uint8_t w02 = r;
				uint8_t rt2 = r;
				if (m == 1) {
					 // write
					w2 = v;
					
					 // write-0
					if (v == 0) {
						w02 = 0;
					}
					
					// toggle;
					rt2 = r ^ v;
				}
				TEST_ASSERT_EQUAL_UINT8(w1 , w2);
				TEST_ASSERT_EQUAL_UINT8(w01, w02);
				TEST_ASSERT_EQUAL_UINT8(rt1, rt2);				
			}
		}
	}
}


uint32_t w_write(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	rw_register = rw_register & (~rw_mask);
	rw_register = rw_register | (value & rw_mask);
	return rw_register;
}

uint32_t w0_write(uint32_t value, uint32_t w0_register, uint32_t w0_mask)
{
	value = value & w0_mask;
	uint32_t w0_register_new = w0_register & value;
	w0_register_new = w0_register_new | ((w0_register & (~w0_mask)));
	return w0_register_new;
}

uint32_t t_set_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r ^ v) ^ r -> r
	// r = 1, v = 1 : (1 ^ 1) ^ 1  = 1   // v=1, r set
	// r = 0, v = 1 : (0 ^ 1) ^ 0  = 1   // v=1, r set
	// r = 1, v = 0 : (1 ^ 0) ^ 1  = 0   // v=0, r not changed
	// r = 0, v = 0 : (0 ^ 0) ^ 0  = 0   // v=0, r not changed
	value = value & t_mask;
	t_register = (t_register ^ value);
	printf("value= %ld, t_register=%ld\n",value, t_register);
	return t_register;
}

uint32_t t_clear_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r & ~v) ^ r -> r
	// r = 1, v = 1 : (1 & ~1) ^ 1  = 1   // v=1, r not changed
	// r = 0, v = 1 : (0 & ~1) ^ 0  = 1   // v=1, r not changed
	// r = 1, v = 0 : (1 & ~0) ^ 1  = 0   // v=0, r cleared
	// r = 0, v = 0 : (0 & ~0) ^ 0  = 0   // v=0, r cleared
	value = value & t_mask;
	t_register = (t_register ^ value);
	printf("value= %ld, t_register=%ld\n",value, t_register);
	return t_register;
}

void test_w(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_write(val, reg, w_mask);

	for (size_t i = 0; i < sizeof(reg); i++)
	{
		TEST_ASSERT_EQUAL_UINT8(w[index], result & 0x01);
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

	uint32_t result = w0_write(val, reg, w0_mask);

	for (size_t i = 0; i < sizeof(reg); i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w0_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w0[index], result & 0x01);
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

}
