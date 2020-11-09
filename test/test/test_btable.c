#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "ep.h"
#include "pma.h"
#include "btable.h"


// 1024 bytes is the longest packet size allowed by USB standard specifications.
const uint16_t max_len = 256; // 1024

void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
#ifdef TEST_ON_TARGET
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	USB->CNTR = USB_CNTR_FRES; // Forse Reset
	USB->CNTR = 0;
	USB->ISTR = 0;
#endif
}

void tearDown(void)
{
}


void _test_btable(const size_t btable_offset, const size_t step_size)
{
	USB->BTABLE = btable_offset; // !! MUST BE ALLIGNED by 0b1000
	volatile struct btable_ep_t *btable_ep = (struct btable_ep_t*)(PMA_ADDRESS) + USB->BTABLE*2;


	for( uint16_t addr_tx = 0; addr_tx <= max_len; addr_tx+=step_size)
		for( uint16_t count_tx = 0; count_tx <= max_len; count_tx+=step_size)
			for( uint16_t addr_rx = 0; addr_rx <= max_len; addr_rx+=step_size)
				for( uint16_t bl_size = 0; bl_size <= 1; bl_size++)
					for( uint16_t num_block = 0; num_block <= 1; num_block++)
						for( uint16_t count_rx = 0; count_rx <= 32; count_rx+=8)
						{
							printf("%08X|%08X|%08X|%01X,%01X,%08X\n", addr_tx, count_tx, addr_rx, bl_size, num_block, count_rx);
							btable_ep[0] = (struct btable_ep_t){
								.tx_addr = addr_tx,
								.tx_count= count_tx,
								.rx_addr = addr_rx,
								.rx_count= {.BL_SIZE_TYPE = bl_size, .NUM_BLOCK = num_block, .COUNT_RX = count_rx,},
							};


							//uint16_t btable_expected[4] = { addr_tx, count_tx, addr_rx, (bl_size << 15) | (num_block << 10) | (count_rx << 0) };
							//uint16_t btable_test[4];
							//read_pma_aligned(USB->BTABLE, &btable_test[0], sizeof(btable_test));
							//TEST_ASSERT_EQUAL_HEX8_ARRAY(btable_expected, btable_test, sizeof(btable_test));

							uint16_t btable_test;
							btable_test = read_pma_u16_aligned(btable_offset);
							TEST_ASSERT_EQUAL_HEX16(addr_tx, btable_test);
							
							btable_test = read_pma_u16_aligned(btable_offset + 2);
							TEST_ASSERT_EQUAL_HEX16(count_tx, btable_test);

							btable_test = read_pma_u16_aligned(btable_offset + 4);
							TEST_ASSERT_EQUAL_HEX16(addr_rx, btable_test);

							btable_test = read_pma_u16_aligned(btable_offset + 6);
							TEST_ASSERT_EQUAL_HEX16((bl_size << 15) | (num_block << 10) | (count_rx << 0) , btable_test);
							//__asm__("bkpt");
						}
}

void test_btable_64(void)
{
	_test_btable(64, 64);
	_test_btable(0, 64);
	/*
	// 64 bytes is length of the whole BTABLE (8 endpoints * (4 * sizeof(uint16_t))
	for (int offset = 0; offset < max_len - 64; offset += 64)
	{
		_test_btable(offset, 64);
	}*/
}
/*
void test_btable_32(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 32)
	{
		_test_btable(offset, 32);
	}
}

void test_btable_16(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 16)
	{
		_test_btable(offset, 16);
	}
}

void test_btable_8(void)
{
	// !! offset must be alligned by 0b1000
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 8);
	}
}

void test_btable_4(void)
{
	// !! offset must be alligned by 0b1000
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 4);
	}
}

void test_btable_2(void)
{
	// !! offset must be alligned by 0b1000
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 2);
	}
}

void test_btable_1(void)
{
	// !! offset must be alligned by 0b1000
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 1);
	}
}
*/
