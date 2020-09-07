#ifndef EP_H
#define EP_H

#define USB_W_MASK  0x0011001100220022
#define USB_W0_MASK 0x0011001100220022
#define USB_T_MASK  0x0011001100220022

#if ((USB_W_MASK & USB_W0_MASK) != 0) || ((USB_W_MASK & USB_T_MASK) != 0) || ((USB_T_MASK & USB_W0_MASK) != 0)
#error "Masks have overlapping bits!"
#endif

#define EP_SET_BITS(EP_VALUE, VALUE_)     set_bits(VALUE_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)
#define EP_CLEAR_BITS(EP_VALUE, VALUE_) clear_bits(VALUE_, EP_VALUE, USB_W_MASK, USB_W0_MASK, USB_T_MASK)

enum ep_direction_t {
	BTABLE_OUT = 0b01,
	BTABLE_IN  = 0b10,
};

#endif // EP_H
