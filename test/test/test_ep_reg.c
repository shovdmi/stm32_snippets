#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f1xx.h"
#include "ep.h"



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

void test_usb_ep0r_rw(void)
{
	TEST_ASSERT_EQUAL_HEX32(RCC->APB1ENR & RCC_APB1ENR_USBEN, RCC_APB1ENR_USBEN);

	uint32_t r = USB->EP0R;
	// zeroing EP0R
	USB->EP0R = r;
	r = USB->EP0R;
	TEST_ASSERT_EQUAL_HEX(0, r);

	r = 0xFFFFU;
	USB->EP0R = r;
	r = USB->EP0R;
	TEST_ASSERT_EQUAL_HEX32(0x777F, r);
}

void test_ep_address(void)
{
	uint16_t addr = 0;
	uint16_t new_addr = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(addr = 0; addr < 0x0F; addr++)
	{
		ep = USB->EP0R;
		ep = ep_write_ep_address(ep, addr);
		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(addr, ep & 0x0F);
		new_addr = ep_get_ep_address(ep);
		TEST_ASSERT_EQUAL_HEX16(addr, new_addr);
	}
}
