#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "pma.h"
#include "btable.h"



#define PMA_MULT (sizeof(pma_uint16_t) / sizeof(uint16_t))

#ifdef TEST_ON_TARGET
#  define PMA_BYTES_NUMBER (256)
#else
   /* 1024 bytes is the longest packet size allowed by USB standard specifications.*/
#  define PMA_BYTES_NUMBER (1024)
#  define PMA_SIZE (PMA_BYTES_NUMBER * PMA_MULT)

   uint8_t pool[PMA_SIZE];
#endif



// stm32f103 has 256 bytes. 1024 bytes is the longest packet size allowed by USB standard specifications.
const uint16_t max_len = PMA_BYTES_NUMBER; 

int log_printf(char *fmt, uint32_t param)
{
	return printf(fmt, param);
}

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
#ifdef TEST_ON_TARGET
	USB->BTABLE = btable_offset; // !! MUST BE ALLIGNED by 0b1000
#endif
	volatile struct btable_ep_t *btable_ep = (uint8_t*)((struct btable_ep_t*)(PMA_ADDRESS)) + btable_offset*2;


	for( uint16_t addr_tx = 0; addr_tx <= max_len; addr_tx+=step_size)
		for( uint16_t count_tx = 0; count_tx <= max_len; count_tx+=step_size)
			for( uint16_t addr_rx = 0; addr_rx <= max_len; addr_rx+=step_size)
				for( uint16_t bl_size = 0; bl_size <= 1; bl_size++)
					for( uint16_t num_block = 0; num_block <= 1; num_block++)
						for( uint16_t count_rx = 0; count_rx <= 32; count_rx+=8)
						{
							//printf("Test values: ");
							//printf("%08X|%08X|%08X|%01X,%01X,%08X\n", addr_tx, count_tx, addr_rx, bl_size, num_block, count_rx);
							btable_ep[0] = (struct btable_ep_t){
								.tx_addr = addr_tx,
								.tx_count= count_tx,
								.rx_addr = addr_rx,
								.rx_count= {.BL_SIZE_TYPE = bl_size, .NUM_BLOCK = num_block, .COUNT_RX = count_rx,},
							};
#if 0
							log_printf("%c", '\n');
							for(size_t i=0; i < PMA_BYTES_NUMBER / PMA_MULT; i++)
							{
								if ( (i & 0x07UL ) == 0) {
									log_printf("%c",'\n');
								}
								else { 
									log_printf("%c",' ');
								}
								pma_uint16_t data = *(((pma_uint16_t*)PMA_ADDRESS) + i);
								log_printf("%02X", data & 0xFF);
								log_printf(" %02X", (data >> 8) & 0xFF);
							}
							log_printf("%c",'\n');*/
#endif
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
	// 64 bytes is length of the whole BTABLE (8 endpoints * (4 * sizeof(uint16_t))
	for (int offset = 0; offset < max_len - 64; offset += 64)
	{
		_test_btable(offset, 64);  // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}

void test_btable_32(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 32)
	{
		_test_btable(offset, 32); // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}

void test_btable_16(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 16)
	{
		_test_btable(offset, 16); // !! offset must be alligned by 0b1000 i.e 8 bytes	
	}
}

void test_btable_8(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 8);  // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}
#if 0
void test_btable_4(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 4); // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}

void test_btable_2(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 2); // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}

void test_btable_1(void)
{
	for (int offset = 0; offset < max_len - 64; offset += 8)
	{
		_test_btable(offset, 1); // !! offset must be alligned by 0b1000 i.e 8 bytes
	}
}
#endif
