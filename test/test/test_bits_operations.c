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

uint8_t set_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  1,  0,  1,  1,  1,      // normal-write bits
};

uint8_t clear_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  1,  0,  0,  1,  0,      // normal-write bits
};

uint8_t toggle_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  1,  0,  1,  1,  0,      // toggled bits
};


uint8_t w_tbl[8] = {
// v |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |    (value to be written)
// r |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |    (register value)
// m |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |    (mask)
        0,  0,  1,  0,  0,  1,  1,  1,      // normal-write bits
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
				uint8_t w_set1= set_tbl[index];
				uint8_t w_clear1= clear_tbl[index];
				uint8_t w01 = w0_tbl[index];
				uint8_t rt1 = t_tbl[index];
				uint8_t rts1 = set_tbl[index];
				uint8_t rtc1 = clear_tbl[index];
				uint8_t w_toggle1= toggle_tbl[index];
				
				uint8_t w2  = r;
				uint8_t w_set2= r;
				uint8_t w_clear2= r;
				uint8_t w02 = r;
				uint8_t rt2 = r;
				uint8_t rts2 = r;
				uint8_t rtc2 = r;
				uint8_t w_toggle2 = r;
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
					
					// write toggleable;
					rt2 = r ^ v;

					// set toggleable;
					rts2 = (v == 1) ? 1 : rts2;

					// clear toggleable;
					rtc2 = (v == 1) ? 0 : rtc2;

          // toggle
					if (v==1) {
             w_toggle2 = (r == 1) ? 0 : 1;
          }
				}
				TEST_ASSERT_EQUAL_HEX8(w1 , w2);
				TEST_ASSERT_EQUAL_HEX8(w_set1 , w_set2);
				TEST_ASSERT_EQUAL_HEX8(w_clear1 , w_clear2);
				TEST_ASSERT_EQUAL_HEX8(w01, w02);
				TEST_ASSERT_EQUAL_HEX8(rt1, rt2);				
				TEST_ASSERT_EQUAL_HEX8(rts1, rts2);				
				TEST_ASSERT_EQUAL_HEX8(rtc1, rtc2);				
				TEST_ASSERT_EQUAL_HEX8(w_toggle1, w_toggle2);
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
	uint32_t expect = 0b00100111;

	uint32_t v_mask = 0xFF;
	uint32_t result = w_write_bits(val, v_mask,reg, w_mask);

	TEST_ASSERT_EQUAL_HEX8(expect, result);

	// bitwise compare
	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(w_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w_mask >>= 1;
	}


	val    = 0b00001111;
	reg    = 0b00110011;
	w_mask = 0b01010101;
	v_mask = 0x00;
	result = w_write_bits(val, v_mask, reg, w_mask);
	TEST_ASSERT_EQUAL_HEX8(reg, result);
}

void test_w_set(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_set_bits(val,reg, w_mask);

	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(set_tbl[index], result & 0x01);
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
		TEST_ASSERT_EQUAL_HEX8(clear_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w_mask >>= 1;
	}
}

void test_w_toggle(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val    = 0b00001111;
	uint32_t reg    = 0b00110011;
	uint32_t w_mask = 0b01010101;

	uint32_t result = w_toggle_bits(val, reg, w_mask);

	for (size_t i = 0; i < 8; i++)
	{
		// index = 0b00000vrm -> 0b0111 -> 0x07
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(toggle_tbl[index], result & 0x01);
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
	uint32_t expect  = 0b00100011;

	uint32_t v_mask = 0xFF;
	uint32_t result = w0_write_bits(val, v_mask, reg, w0_mask);

	TEST_ASSERT_EQUAL_HEX8(expect, result);

	// Bitwise compare
	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (w0_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(w0_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		w0_mask >>= 1;
	}

	val    = 0b00001111;
	reg    = 0b00110011;
	w0_mask = 0b01010101;
	v_mask = 0x00;
	result = w0_write_bits(val, v_mask, reg, w0_mask);
	TEST_ASSERT_EQUAL_HEX8(reg, result);
}


void test_t(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;
	uint32_t expect  = 0b00110110; // treat all the bits as read-write except ones masked as toggleable, i.e. we want to keep their values

	uint32_t v_mask = 0xFF;
	uint32_t result = t_write_bits(val, v_mask, reg, t_mask);

	TEST_ASSERT_EQUAL_HEX8(expect, result);

	// Bitwise compare
	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(t_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}

	val    = 0b00001111;
	reg    = 0b00110011;
	t_mask = 0b01010101;
	v_mask = 0x00;
	expect = 0b00100010; // treat all the bits as read-write except ones masked as toggleable, i.e. we want to keep their values

	result = t_write_bits(val, v_mask, reg, t_mask);
	TEST_ASSERT_EQUAL_HEX8(expect, result); // we write 0's to toggleable bits in order to keep register value
}

void test_t_set_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;
//     final value = 0br0r1r1r1   r-keep register bit where m==0
//     final value = 0b00110111   == 0x37
// to be written   = 0b00100110   == 0x26
	uint32_t result = t_set_bits(val, reg, t_mask);

	// Should work both way
	//result = reg  ^ (result & t_mask); // Emulating hardware-XOR-ing
	result = (reg & t_mask) ^ result; // Emulating hardware-XOR-ing

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(set_tbl[index], result & 0x01);
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
//     final value = 0br0r1r0r0   r-keep register bit where m==0
//     final value = 0b00110010   == 0x32
// to be written   = 0b00100010   == 0x22

	uint32_t result = t_clear_bits(val, reg, t_mask);
	result = (reg & t_mask)  ^ result; // Emulating hardware-XOR-ing

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(clear_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}

void test_t_toggle_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t t_mask  = 0b01010101;
//     final value = 0br0r1r1r0   r-keep register bit where m==0
//     final value = 0b00110110   == 0x36
// to be xor-ed    = 0b00000101   == 0x05
// to be written   = 0b00100111   == 0x27

	uint32_t result = t_toggle_bits(val, reg, t_mask);
	result = (reg & t_mask)  ^ result; // Emulating hardware-XOR-ing

	for (size_t i = 0; i < 8; i++)
	{
		size_t index = ((val & 0x01) << 2) | ((reg & 0x01) << 1) | (t_mask & 0x01);
		TEST_ASSERT_EQUAL_HEX8(toggle_tbl[index], result & 0x01);
		result >>= 1;
		val >>= 1;
		reg >>= 1;
		t_mask >>= 1;
	}
}

void test_set_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t mask    = 0b01010101;

	uint32_t w_val = val;
	uint32_t w_reg = reg;
	uint32_t w_mask = mask;

	uint32_t w0_val = val << 8;
	uint32_t w0_reg = reg << 8;
	uint32_t w0_mask = mask << 8;

	uint32_t t_val = val << 16;
	uint32_t t_reg = reg << 16;
	uint32_t t_mask = mask << 16;

	// part of the 32bit number which must be keep
	uint32_t k_val = val << 24;
	uint32_t k_reg = reg << 24;
	uint32_t k_mask = 0;


	uint32_t g_val =  t_val            | w_val  | k_val; // we omit w0_val here because we cannot set write-zero bits
	uint32_t g_reg =  t_reg  | w0_reg  | w_reg  | k_reg;
	uint32_t g_mask = t_mask | w0_mask | w_mask | k_mask;

	uint32_t expected_w = w_set_bits(w_val, g_reg, w_mask);
	uint32_t expected = (w_val & w_mask) | g_reg;
	uint32_t result = set_bits(w_val, g_reg, w_mask, w0_mask, t_mask);
	result = result ^ (g_reg & t_mask);
	TEST_ASSERT_EQUAL_HEX32(expected, result);

	expected = (t_val & t_mask) | g_reg;
	result = set_bits(t_val, g_reg, w_mask, w0_mask, t_mask);
	result = result ^ (g_reg & t_mask);
	TEST_ASSERT_EQUAL_HEX32(expected, result);

	expected = (g_val & g_mask) | g_reg;
	//printf("expected reg  =0x%08X\n", expected);
	result = set_bits(g_val, g_reg, w_mask, w0_mask, t_mask);
	//printf("result        =0x%08X\n", result);
	result = result ^ (g_reg & t_mask);
	//printf("t_result      =0x%08X\n", result);
	TEST_ASSERT_EQUAL_HEX32(expected, result);
}


void test_clear_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t mask    = 0b01010101;

	uint32_t w_val = val;
	uint32_t w_reg = reg;
	uint32_t w_mask = mask;

	uint32_t w0_val = val << 8;
	uint32_t w0_reg = reg << 8;
	uint32_t w0_mask = mask << 8;

	uint32_t t_val = val << 16;
	uint32_t t_reg = reg << 16;
	uint32_t t_mask = mask << 16;

	// part of the 32bit number which must be keep
	uint32_t k_val = val << 24;
	uint32_t k_reg = reg << 24;
	uint32_t k_mask = 0;

	uint32_t g_val =  t_val | w0_val | w_val | k_val;
	uint32_t g_reg =  t_reg | w0_reg | w_reg | k_reg;
	uint32_t g_mask = t_mask| w0_mask| w_mask| k_mask;

	uint32_t expected = (~(g_val & g_mask)) & g_reg;
	//printf("g_reg =  0x%08X \nexpected mask =0x%08X\n", g_reg, ~(g_val & g_mask));
	//printf("expected reg  =0x%08X\n", expected);
	//printf("t_expected reg=0x%08X\n", expected ^ t_mask);
	uint32_t result = clear_bits(g_val, g_reg, w_mask, w0_mask, t_mask);
	//printf("result        =0x%08X\n", result);
	result = result ^ (g_reg & t_mask);
	//printf("t_result      =0x%08X\n", result);
	TEST_ASSERT_EQUAL_HEX32(expected, result);
}

void test_toggle_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t mask    = 0b01010101;

	uint32_t w_val = val;
	uint32_t w_reg = reg;
	uint32_t w_mask = mask;

	uint32_t w0_val = val << 8;
	uint32_t w0_reg = reg << 8;
	uint32_t w0_mask = mask << 8;

	uint32_t t_val = val << 16;
	uint32_t t_reg = reg << 16;
	uint32_t t_mask = mask << 16;

	// part of the 32bit number which must be keep
	uint32_t k_val = val << 24;
	uint32_t k_reg = reg << 24;
	uint32_t k_mask = 0;

	uint32_t g_val =  t_val | w0_val | w_val | k_val;
	uint32_t g_reg =  t_reg | w0_reg | w_reg | k_reg;
	uint32_t g_mask = t_mask| w0_mask| w_mask| k_mask;

	uint32_t expected = 0x33363636;
	uint32_t result = toggle_bits(g_val, g_reg, w_mask, w0_mask, t_mask);
	result = result ^ (g_reg & t_mask);
	TEST_ASSERT_EQUAL_HEX32(expected, result);
}

void test_write_bits(void)
{
	// bits of val, reg and mask here repeat values in the 'rw', 'w0' and 't' tables
	uint32_t val     = 0b00001111;
	uint32_t reg     = 0b00110011;
	uint32_t mask    = 0b01010101;

	uint32_t w_val = val;
	uint32_t w_reg = reg;
	uint32_t w_mask = mask;

	uint32_t w0_val = val << 8;
	uint32_t w0_reg = reg << 8;
	uint32_t w0_mask = mask << 8;

	uint32_t t_val = val << 16;
	uint32_t t_reg = reg << 16;
	uint32_t t_mask = mask << 16;

	// part of the 32bit number which must be keep
	uint32_t k_val = val << 24;
	uint32_t k_reg = reg << 24;
	uint32_t k_mask = 0;

	uint32_t g_val =  t_val | w0_val | w_val | k_val;
	uint32_t g_reg =  t_reg | w0_reg | w_reg | k_reg;
	uint32_t g_mask = t_mask| w0_mask| w_mask| k_mask;

	uint32_t v_mask = 0xFFFFFFFF;
	uint32_t expected = 0x33272327;
	uint32_t result = write_bits(g_val, v_mask, g_reg, w_mask, w0_mask, t_mask);
	result = result ^ (g_reg & t_mask);
	TEST_ASSERT_EQUAL_HEX32(expected, result);

	v_mask = 0x00000000;
	expected = g_reg;
	result = write_bits(g_val, v_mask, g_reg, w_mask, w0_mask, t_mask);
	result = result ^ (g_reg & t_mask);
	TEST_ASSERT_EQUAL_HEX32(expected, result);
}
