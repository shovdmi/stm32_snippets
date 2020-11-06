#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void test_ep_address(void)
{
	uint16_t addr = 0;
	uint16_t new_addr = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(addr = 0; addr < 0x0F; addr++)
	{
		USB->EP0R = 0;

		ep = USB->EP0R;
		ep = ep_write_ep_address(ep, addr);
		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(addr, ep & 0x0F);

		new_addr = ep_get_ep_address(ep);
		TEST_ASSERT_EQUAL_HEX16(addr, new_addr);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void test_ep_type_write(void)
{
	uint16_t type = 0;
	uint16_t new_type = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(type = 0; type < 0x04; type++)
	{
		USB->EP0R = 0;

		ep = USB->EP0R;
		ep = ep_write_ep_type(ep, type);
		TEST_ASSERT_EQUAL_HEX16(type << USB_EP_T_FIELD_Pos, ep & USB_EP_T_FIELD_Msk);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(type << USB_EP_T_FIELD_Pos, ep & USB_EP_T_FIELD_Msk);

		new_type = ep_get_ep_type(ep);
		TEST_ASSERT_EQUAL_HEX16(type, new_type);
	}
}

void test_ep_type_set_clear(void)
{
	uint16_t type = 0;
	uint16_t new_type = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(type = 0; type < 0x04; type++)
	{
		USB->EP0R = 0;

		// setting the bits
		ep = USB->EP0R;
		ep = ep_set_ep_type(ep, type);
		TEST_ASSERT_EQUAL_HEX16(type << USB_EP_T_FIELD_Pos, type << USB_EP_T_FIELD_Pos & ep);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(type << USB_EP_T_FIELD_Pos, type << USB_EP_T_FIELD_Pos & ep);

		new_type = ep_get_ep_type(ep);
		TEST_ASSERT_EQUAL_HEX16(type, new_type);

		// clearing the bits
		ep = USB->EP0R;
		uint16_t new_ep = ep_clear_ep_type(ep, type);
		TEST_ASSERT_EQUAL_HEX16((ep & (~(type  << USB_EP_T_FIELD_Pos))) & USB_EP_T_FIELD_Msk, new_ep & USB_EP_T_FIELD_Msk);
	}
}

void test_ep_type_toggle(void)
{
	// TODO:...
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void test_ep_kind_write(void)
{
	uint16_t val = 0;
	uint16_t new_val = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(val = 0; val < 0x02; val++)
	{
		ep = USB->EP0R;
		ep = ep_write_ep_kind(ep, val);
		TEST_ASSERT_EQUAL_HEX16(val << USB_EP_KIND_Pos, ep & USB_EP_KIND_Msk);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(val << USB_EP_KIND_Pos, ep & USB_EP_KIND_Msk);

		new_val = ep_get_ep_kind(ep);
		TEST_ASSERT_EQUAL_HEX16(val, new_val);
	}
}

void test_ep_kind_set_clear(void)
{
	uint16_t value = 0;
	uint16_t new_value = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(value = 0; value < 0x02; value++)
	{
		USB->EP0R = 0;

		// setting the bits
		ep = USB->EP0R;
		ep = ep_set_ep_kind(ep, value);
		TEST_ASSERT_EQUAL_HEX16(value << USB_EP_KIND_Pos, value << USB_EP_KIND_Pos & ep);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(value << USB_EP_KIND_Pos, value << USB_EP_KIND_Pos & ep);

		new_value = ep_get_ep_kind(ep);
		TEST_ASSERT_EQUAL_HEX16(value, new_value);

		// clearing the bits
		ep = USB->EP0R;
		uint16_t new_ep = ep_clear_ep_kind(ep, value);
		TEST_ASSERT_EQUAL_HEX16((ep & (~(value  << USB_EP_KIND_Pos))) & USB_EP_KIND_Msk, new_ep & USB_EP_KIND_Msk);
	}
}

void test_ep_kind_toggle(void)
{
	// TODO:...
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void test_stat_rx_write(void)
{
	uint16_t val = 0;
	uint16_t new_val = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(val = 0; val < 0x04; val++)
	{
		ep = USB->EP0R;
		ep = ep_write_stat_rx(ep, val);
		//TEST_ASSERT_EQUAL_HEX16(val << USB_EP_stat_RX_Pos, ep & USB_EP_stat_RX_Msk);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(val << USB_EPRX_STAT_Pos, ep & USB_EPRX_STAT_Msk);

		new_val = ep_get_stat_rx(ep);
		TEST_ASSERT_EQUAL_HEX16(val, new_val);
	}
}

void test_stat_rx_set_clear(void)
{
	uint16_t value = 0;
	uint16_t new_value = 0xFFFF;
	uint16_t ep = 0xFFFF;

	for(value = 0; value < 0x04; value++)
	{
		USB->EP0R = 0;

		// setting the bits
		ep = USB->EP0R;
		ep = ep_set_stat_rx(ep, value);
		TEST_ASSERT_EQUAL_HEX16(value << USB_EPRX_STAT_Pos, (value << USB_EPRX_STAT_Pos) & ep);

		USB->EP0R = ep;

		ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16(value << USB_EPRX_STAT_Pos, (value << USB_EPRX_STAT_Pos) & ep);

		new_value = ep_get_stat_rx(ep);
		TEST_ASSERT_EQUAL_HEX16(value << USB_EPRX_STAT_Pos, (new_value << USB_EPRX_STAT_Pos) & ep);

		// clearing the bits
		ep = USB->EP0R;
		uint16_t new_ep = ep_clear_stat_rx(ep, value);
		USB->EP0R = new_ep;
		new_ep = USB->EP0R;
		TEST_ASSERT_EQUAL_HEX16((ep & (~(value  << USB_EPRX_STAT_Pos))) & USB_EPRX_STAT_Msk, new_ep & USB_EPRX_STAT_Msk);
	}
}

void test_stat_rx_toggle(void)
{
	// TODO:...
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
