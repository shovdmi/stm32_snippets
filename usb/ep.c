//#include "debug.h"
#include <stdint.h>
#include "ep.h"
#include "bits_operation.h"

#ifdef TEST_ON_TARGET
#define ASSERT(...)
#endif

/* bit positions */ 
#define USB_EP_CTR_RX_Pos                       (15U)                          
#define USB_EP_CTR_RX_Msk                       (0x1U << USB_EP_CTR_RX_Pos)    /*!< 0x00008000 */
#define USB_EP_CTR_RX                           USB_EP_CTR_RX_Msk              /*!< EndPoint Correct TRansfer RX */
#define USB_EP_DTOG_RX_Pos                      (14U)                          
#define USB_EP_DTOG_RX_Msk                      (0x1U << USB_EP_DTOG_RX_Pos)   /*!< 0x00004000 */
#define USB_EP_DTOG_RX                          USB_EP_DTOG_RX_Msk             /*!< EndPoint Data TOGGLE RX */
#define USB_EPRX_STAT_Pos                       (12U)                          
#define USB_EPRX_STAT_Msk                       (0x3U << USB_EPRX_STAT_Pos)    /*!< 0x00003000 */
#define USB_EPRX_STAT                           USB_EPRX_STAT_Msk              /*!< EndPoint RX STATus bit field */
#define USB_EP_SETUP_Pos                        (11U)                          
#define USB_EP_SETUP_Msk                        (0x1U << USB_EP_SETUP_Pos)     /*!< 0x00000800 */
#define USB_EP_SETUP                            USB_EP_SETUP_Msk               /*!< EndPoint SETUP */
#define USB_EP_T_FIELD_Pos                      (9U)                           
#define USB_EP_T_FIELD_Msk                      (0x3U << USB_EP_T_FIELD_Pos)   /*!< 0x00000600 */
#define USB_EP_T_FIELD                          USB_EP_T_FIELD_Msk             /*!< EndPoint TYPE */
#define USB_EP_KIND_Pos                         (8U)                           
#define USB_EP_KIND_Msk                         (0x1U << USB_EP_KIND_Pos)      /*!< 0x00000100 */
#define USB_EP_KIND                             USB_EP_KIND_Msk                /*!< EndPoint KIND */
#define USB_EP_CTR_TX_Pos                       (7U)                           
#define USB_EP_CTR_TX_Msk                       (0x1U << USB_EP_CTR_TX_Pos)    /*!< 0x00000080 */
#define USB_EP_CTR_TX                           USB_EP_CTR_TX_Msk              /*!< EndPoint Correct TRansfer TX */
#define USB_EP_DTOG_TX_Pos                      (6U)                           
#define USB_EP_DTOG_TX_Msk                      (0x1U << USB_EP_DTOG_TX_Pos)   /*!< 0x00000040 */
#define USB_EP_DTOG_TX                          USB_EP_DTOG_TX_Msk             /*!< EndPoint Data TOGGLE TX */
#define USB_EPTX_STAT_Pos                       (4U)                           
#define USB_EPTX_STAT_Msk                       (0x3U << USB_EPTX_STAT_Pos)    /*!< 0x00000030 */
#define USB_EPTX_STAT                           USB_EPTX_STAT_Msk              /*!< EndPoint TX STATus bit field */
#define USB_EPADDR_FIELD_Pos                    (0U)                           
#define USB_EPADDR_FIELD_Msk                    (0xFU << USB_EPADDR_FIELD_Pos) /*!< 0x0000000F */
#define USB_EPADDR_FIELD                        USB_EPADDR_FIELD_Msk           /*!< EndPoint ADDRess FIELD */
/*
inline uint16_t ep_read_value(uint8_t ep_number)
{
	volatile uint32_t *ep_ptr = (uint32_t*)&USB->EP0R;
	uint16_t ep_value = ep_ptr[ep_number];
	return ep_value;
}

inline void ep_write_value(uint8_t ep_number, uint16_t value)
{
	volatile uint32_t *ep_ptr = (uint32_t*)&USB->EP0R;
	// TODO: is there 32-bit access either on F103 and on L053??
	ep_ptr[ep_number] = (uint32_t)value;
}

#if 0
inline void ep_set_zero(uint8_t ep_number)
{
	volatile uint32_t *ep_ptr = (uint32_t*)&USB->EP0R;
	// TODO: is there 32-bit access either on F103 and on L053??
	ep_ptr[ep_number] = (uint32_t)0;       // write zero to w0-bits and rw-bits
	ep_ptr[ep_number] = ep_ptr[ep_number]; // write zero to t-bits, as x ^ x == 0
}

// DO NOT USE this if you calculate ep_value using functions defined down below.
// USE ep_write_value() instead!
inline void ep_set_value(uint8_t ep_number, uint16_t value)
{
	assert(false);

	ep_make_zero(ep_number);
	ep_write_value(ep_number, value);
}
#endif

*/
/* -------------------------------------------------------------------------- */

/* here we assume that one calls functions below using the value of Endpoint register as a parameter
 * i.e. : ep_value = *(EP[ep_num].EPnR);
*/
uint16_t ep_write_ep_address(uint16_t ep_value, uint8_t address)
{
	ASSERT(((address << USB_EPADDR_FIELD_Pos) & USB_EPADDR_FIELD_Msk) == (address << USB_EPADDR_FIELD_Pos));

	//ep_value = write_bits(address, USB_EPADDR_FIELD_Msk, ep_value, USB_W_MASK, USB_W0_MASK, USB_T_MASK);
	ep_value = EP_WRITE_BITS(ep_value, address << USB_EPADDR_FIELD_Pos, USB_EPADDR_FIELD_Msk);
	return ep_value;
}

uint16_t ep_get_ep_address(uint16_t ep_value)
{
	return (ep_value & USB_EPADDR_FIELD_Msk) >> USB_EPADDR_FIELD_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_get_setup(uint16_t ep_value)
{
	return (ep_value & USB_EP_SETUP_Msk) >> USB_EP_SETUP_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

uint16_t ep_set_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

uint16_t ep_clear_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

uint16_t ep_toggle_ep_type(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_T_FIELD_Pos;
	ASSERT((value & USB_EP_T_FIELD_Msk) == value);
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_T_FIELD_Msk);
	return ep_value;
}

uint16_t ep_get_ep_type(uint16_t ep_value)
{
	return (ep_value & USB_EP_T_FIELD_Msk) >> USB_EP_T_FIELD_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

uint16_t ep_set_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

uint16_t ep_clear_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

uint16_t ep_toggle_ep_kind(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_KIND_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_KIND_Msk);
	return ep_value;
}

uint16_t ep_get_ep_kind(uint16_t ep_value)
{
	return (ep_value & USB_EP_KIND_Msk) >> USB_EP_KIND_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

uint16_t ep_set_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

uint16_t ep_clear_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

uint16_t ep_toggle_ctr_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_RX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_CTR_RX_Msk);
	return ep_value;
}

uint16_t ep_get_ctr_rx(uint16_t ep_value)
{
	return (ep_value & USB_EP_CTR_RX_Msk) >> USB_EP_CTR_RX_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

uint16_t ep_set_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

uint16_t ep_clear_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

uint16_t ep_toggle_ctr_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_CTR_TX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_CTR_TX_Msk);
	return ep_value;
}

uint16_t ep_get_ctr_tx(uint16_t ep_value)
{
	return (ep_value & USB_EP_CTR_TX_Msk) >> USB_EP_CTR_TX_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

uint16_t ep_set_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

uint16_t ep_clear_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

uint16_t ep_toggle_ep_dtog_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_RX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_DTOG_RX_Msk);
	return ep_value;
}

uint16_t ep_get_ep_dtog_rx(uint16_t ep_value)
{
	return (ep_value & USB_EP_DTOG_RX_Msk) >> USB_EP_DTOG_RX_Pos;
}
/* -------------------------------------------------------------------------- */
uint16_t ep_write_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

uint16_t ep_set_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

uint16_t ep_clear_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

uint16_t ep_toggle_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EP_DTOG_TX_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EP_DTOG_TX_Msk);
	return ep_value;
}

uint16_t ep_get_ep_dtog_tx(uint16_t ep_value, uint16_t value)
{
	return (ep_value & USB_EP_DTOG_TX_Msk) >> USB_EP_DTOG_TX_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

uint16_t ep_set_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

uint16_t ep_clear_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

uint16_t ep_toggle_stat_rx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPRX_STAT_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EPRX_STAT_Msk);
	return ep_value;
}

uint16_t ep_get_stat_rx(uint16_t ep_value)
{
	return (ep_value & USB_EPRX_STAT_Msk) >> USB_EPRX_STAT_Pos;
}

/* -------------------------------------------------------------------------- */
uint16_t ep_write_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_WRITE_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

uint16_t ep_set_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_SET_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

uint16_t ep_clear_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_CLEAR_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

uint16_t ep_toggle_stat_tx(uint16_t ep_value, uint16_t value)
{
	value = value << USB_EPTX_STAT_Pos;
	ep_value = EP_TOGGLE_BITS(ep_value, value, USB_EPTX_STAT_Msk);
	return ep_value;
}

uint16_t ep_get_stat_tx(uint16_t ep_value)
{
	return (ep_value & USB_EPTX_STAT_Msk) >> USB_EPTX_STAT_Pos;
}



/* -------------------------------------------------------------------------- */

/*
inline uint16_t ep_set_direction(uint16_t ep_value, enum ep_direction_t direction)
{
	// TODO make a single command assert here
	if ((direction & BTABLE_OUT) && (direction & BTABLE_IN))
		if (ep_get_setup(ep_value) == 0)
			assert(false);

	// Do we need this?
	ep_value ^ = USB_EP_RX_NAK;
	ep_value ^ = USB_EP_TX_NAK;

	if (direction & BTABLE_IN) {
		ep_value ^= USB_EP_RX_VALID;
	}

	if (direction & BTABLE_OUT) {
		ep_value ^= USB_EP_TX_VALID;
	}

	return ep_value;
}


inline uint16_t ep_set_type(uint16_t ep_value, enum ep_type_t ep_type)
{
	switch (ep_type)
	{
		case USB_EP_CONTROL:
			break;

		case USB_EP_INTERRUPT:
			break;

		case USB_EP_BULK:
			break;

		case USB_EP_ISOCHRONOUS:
				ep_value |= USB_EP_ISOCHRONOUS;
			break;

		default:
			break;
	}

	return ep_value;
}*/
