#ifndef EP_H
#define EP_H

//#include "debug.h"
#include <stdint.h>
#include "stm32f1xx.h"
#include "bits_operation.h"

#ifdef TEST_ON_TARGET
#define ASSERT(...)
#endif

#define USB_W_MASK  (USB_EP_T_FIELD_Msk | USB_EP_KIND_Msk | USB_EPADDR_FIELD_Msk)
#define USB_W0_MASK (USB_EP_CTR_RX_Msk | USB_EP_CTR_TX_Msk)
#define USB_T_MASK  (USB_EP_DTOG_RX_Msk | USB_EPRX_STAT_Msk | USB_EP_DTOG_TX_Msk | USB_EPTX_STAT_Msk)
#define USB_RO_MASK (USB_EP_SETUP_Msk)

#if ((USB_W_MASK & USB_W0_MASK) != 0) || ((USB_W_MASK & USB_T_MASK) != 0) || ((USB_T_MASK & USB_W0_MASK) != 0)
#error "Masks have overlapping bits!"
#endif

#if ((USB_W_MASK & USB_R_MASK) != 0) || ((USB_W0_MASK & USB_RO_MASK) != 0) || ((USB_T_MASK & USB_RO_MASK) != 0)
#error "Masks and Read-only mask have overlapping bits!"
#endif

#if ((USB_W_MASK | USB_W0_MASK | USB_T_MASK | USB_RO_MASK) != 0xFFFF)
#error "Mask's bits missing!"
#endif

#define EP_WRITE_BITS(EP_VALUE, VALUE_, VALUE_MASK_)   write_bits(VALUE_, VALUE_MASK_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)
#define EP_SET_BITS(EP_VALUE, VALUE_, VALUE_MASK_)       set_bits(VALUE_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)
#define EP_CLEAR_BITS(EP_VALUE, VALUE_, VALUE_MASK_)   clear_bits(VALUE_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)
#define EP_TOGGLE_BITS(EP_VALUE, VALUE_, VALUE_MASK_) toggle_bits(VALUE_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)

/* -------------------------------------------------------------------------- */

/* here we assume that one calls functions below using the value of Endpoint register as a parameter
 * i.e. : ep_value = *(EP[ep_num].EPnR);
*/
static inline uint16_t ep_write_ep_address(uint16_t ep_value, uint8_t address)
{
	ASSERT(((address << USB_EPADDR_FIELD_Pos) & USB_EPADDR_FIELD_Msk) == (address << USB_EPADDR_FIELD_Pos));

	//ep_value = write_bits(address, USB_EPADDR_FIELD_Msk, ep_value, USB_W_MASK, USB_W0_MASK, USB_T_MASK);
	ep_value = EP_WRITE_BITS(ep_value, address << USB_EPADDR_FIELD_Pos, USB_EPADDR_FIELD_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ep_address(uint16_t ep_value)
{
	return (ep_value & USB_EPADDR_FIELD_Msk) >> USB_EPADDR_FIELD_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_get_setup(uint16_t ep_value)
{
	return (ep_value & USB_EP_SETUP_Msk) >> USB_EP_SETUP_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ep_type(uint16_t ep_value)
{
	return (ep_value & USB_EP_T_FIELD_Msk) >> USB_EP_T_FIELD_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ep_kind(uint16_t ep_value)
{
	return (ep_value & USB_EP_KIND_Msk) >> USB_EP_KIND_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ctr_rx(uint16_t ep_value)
{
	return (ep_value & USB_EP_CTR_RX_Msk) >> USB_EP_CTR_RX_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ctr_tx(uint16_t ep_value)
{
	return (ep_value & USB_EP_CTR_TX_Msk) >> USB_EP_CTR_TX_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ep_dtog_rx(uint16_t ep_value)
{
	return (ep_value & USB_EP_DTOG_RX_Msk) >> USB_EP_DTOG_RX_Pos;
}
/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_set_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

static inline uint16_t ep_get_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	return (ep_value & USB_EP_DTOG_TX_Msk) >> USB_EP_DTOG_TX_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_set_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_get_stat_rx(uint16_t ep_value)
{
	return (ep_value & USB_EPRX_STAT_Msk) >> USB_EPRX_STAT_Pos;
}

/* -------------------------------------------------------------------------- */
static inline uint16_t ep_write_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_set_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_clear_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_toggle_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

static inline uint16_t ep_get_stat_tx(uint16_t ep_value)
{
	return (ep_value & USB_EPTX_STAT_Msk) >> USB_EPTX_STAT_Pos;
}

#endif // EP_H
