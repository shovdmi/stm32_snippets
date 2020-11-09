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

enum usb_stat_t {
	STAT_DISABLED = 0,
	STAT_STALL =    1,
	STAT_NAK =      2,
	STAT_VALID =    3,
};

enum usb_ep_type_kind_t {
	EP_TYPE_BULK =               0, // (0 << 1) | 0,
	EP_TYPE_BULK_DBL_BUF=        1, // (0 << 1) | 1,
	EP_TYPE_CONTROL =            2, // (1 << 1) | 0,
	EP_TYPE_CONTROL_STATUS_OUT = 3, // (1 << 1) | 1,
	EP_TYPE_ISO =                4, // (2 << 1) | 0,
	EP_TYPE_INTERRUPT =          6, // (3 << 1) | 0,
};



void test_init_endpoint(void)
{
	uint16_t ep = USB->EP0R;
	ep = ep_write_ep_type_kind(ep, EP_TYPE_CONTROL);
	USB->EP0R = ep;

	__asm__("bkpt");
}
