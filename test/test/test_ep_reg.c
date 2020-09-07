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
        0,  0,  1,  1,  0,  1,  1,  0,      // toggleable bits
};

uint8_t t_set[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |
        0,  0,  1,  1,  0,  1,  1,  1,      // set toggleable bits
};

uint8_t t_clear[8] = {
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
				
				uint8_t w1  = w[index];
				uint8_t w_set1= w_set_tbl[index];
				uint8_t w_clear1= w_clear_tbl[index];
				uint8_t w01 = w0[index];
				uint8_t rt1 = t[index];
				uint8_t rts1 = t_set[index];
				uint8_t rtc1 = t_clear[index];
				
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


uint32_t w_write(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	rw_register = rw_register & (~rw_mask);
	rw_register = rw_register | (value & rw_mask);
	return rw_register;
}

uint32_t w_set(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	value = value & rw_mask;
	rw_register = rw_register | value;
	return rw_register;
}

uint32_t w_clear(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	value = value & rw_mask;
	rw_register = rw_register & (~value);
	return rw_register;
}

uint32_t w0_write(uint32_t value, uint32_t w0_register, uint32_t w0_mask)
{
	value = value & w0_mask;
	uint32_t w0_register_new = w0_register & value;
	w0_register_new = w0_register_new | ((w0_register & (~w0_mask)));
	return w0_register_new;
}

uint32_t t_write(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	value = value & t_mask;
	uint32_t t_register_new = t_register ^ value;
	return t_register_new;
}

uint32_t t_set_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r ^ v) ^ r -> r
	// r = 1, v = 1 : (1 ^ 1) ^ 1  = 1   // v=1, r set
	// r = 0, v = 1 : (0 ^ 1) ^ 0  = 1   // v=1, r set
	// r = 1, v = 0 : (1 ^ 0) ^ 1  = 0   // v=0, r not changed
	// r = 0, v = 0 : (0 ^ 0) ^ 0  = 0   // v=0, r not changed
	//printf("value= 0x%08X, t_register=0x%08X\n",value, x_value);
	value = value & t_mask;
	// (reg OR val) is a required value of 'reg' after hw-XOR-ing a looked for number
	// i.e. we need to make a preceeding XOR to get a number which will be XOR-ed with 'reg' by hardware
	// (t_register | value) ^ t_register  ^(by hw)  t_register  == (t_register | value)
	uint32_t x_value = (t_register | value) ^ t_register;
	return x_value;
}

uint32_t t_clear_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r & v) ^ r -> r
	// r = 1, v = 0 : (1 & 0) ^ 1  = 1   // v=1, r not changed
	// r = 0, v = 0 : (0 & 0) ^ 0  = 1   // v=1, r not changed
	// r = 1, v = 1 : (1 & 1) ^ 1  = 0   // v=0, r cleared
	// r = 0, v = 1 : (0 & 1) ^ 0  = 0   // v=0, r cleared

	// if m == 0 and either v == 0 or v == 1, we don't touch r
	// if m == 1 and v == 1, we reset r.
	// For example we want to reset bits using value=0b1010 and mask=0b1100. That means
	// our masked 'value' is going to be 0b1000.
	// For clearing a bit we use bitwise-AND between r and 1 (one) preceeding hardware bitwise-XOR,
	// i.e (r & 1) == r ----> (this xor is made by usb-core) r ^ r = 0
	value = value & t_mask;
	uint32_t x_value = t_register & value;
	return x_value;
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
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(w[index], result & 0x01);
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

	uint32_t result = w_set(val, reg, w_mask);

	for (size_t i = 0; i < sizeof(reg); i++)
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

	uint32_t result = w_clear(val, reg, w_mask);

	for (size_t i = 0; i < sizeof(reg); i++)
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

	uint32_t result = t_write(val, reg, t_mask);

	for (size_t i = 0; i < sizeof(reg); i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t[index], result & 0x01);
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

	for (size_t i = 0; i < sizeof(reg); i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t_set[index], result & 0x01);
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

	for (size_t i = 0; i < sizeof(reg); i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_UINT8(t_clear[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}
