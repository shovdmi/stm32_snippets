#ifndef EP_H
#define EP_H

/*
5432 1098 7654 3210
0111 0000 0111 0000 -- t      0x7070
1000 0000 1000 0000 -- rc_w0  0x8080
0000 0111 0000 1111 -- rw     0x070F
0000 1000 0000 0000 -- r      0x0800
*/

#define USB_W_MASK  0x070F
#define USB_W0_MASK 0x8080
#define USB_T_MASK  0x7070
#define USB_RO_MASK  0x0800

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

enum ep_direction_t {
	BTABLE_OUT = 0b01,
	BTABLE_IN  = 0b10,
};

#endif // EP_H
