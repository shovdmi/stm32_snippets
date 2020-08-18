#include "ep.h"

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

inline void ep_make_zero(uint8_t ep_number)
{
	volatile uint32_t *ep_ptr = (uint32_t*)&USB->EP0R;
	// TODO: is there 32-bit access either on F103 and on L053??
	ep_ptr[ep_number] = (uint32_t)0;       // write zero to w0-bits and rw-bits
	ep_ptr[ep_number] = ep_ptr[ep_number]; // write zero to t-bits, as x ^ x == 0
}

inline void ep_set_value(uint8_t ep_number, uint16_t value)
{
	ep_make_zero(ep_number);
	ep_write_value(ep_number, value);
}


// here we assume that user calls using the value of Endpoint register as a parameter
// i.e. : ep_value = *(EP[ep_num].EPnR);
inline uint16_t ep_set_ep_address(uint16_t ep_value, uint8_t address)
{
	assert((address & 0x0F) == address);
	ep_value |= address;
	return ep_value;
}

inline uint16_t ep_set_setup(uint16_t ep_value)
{
		ep_value |= USB_EP_CONTROL;
		return ep_value
}

inline uint16_t ep_clear_setup(uint16_t ep_value)
{
		ep_value &= (uint16_t)~USB_EP_CONTROL;
		return ep_value
}

inline uint16_t ep_get_setup(uint16_t ep_value)
{
		return !!(ep_value & USB_EP_CONTROL);
}

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
}
