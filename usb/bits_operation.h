#ifndef BITS_OPERATION_H
#define BITS_OPERATION_H

#include <stdint.h>

static __inline__ uint32_t w_write_bits(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	rw_register = rw_register & (~rw_mask);
	rw_register = rw_register | (value & rw_mask);
	return rw_register;
}

static __inline__ uint32_t w_set_bits(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	value = value & rw_mask;
	rw_register = rw_register | value;
	return rw_register;
}

static __inline__ uint32_t w_clear_bits(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	value = value & rw_mask;
	rw_register = rw_register & (~value);
	return rw_register;
}

static __inline__ uint32_t w0_write_bits(uint32_t value, uint32_t w0_register, uint32_t w0_mask)
{
	value = value & w0_mask;
	uint32_t w0_register_new = w0_register & value;
	w0_register_new = w0_register_new | ((w0_register & (~w0_mask)));
	return w0_register_new;
}

static __inline__ uint32_t t_write_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	value = value & t_mask;
	uint32_t t_register_new = t_register ^ value;
	return t_register_new;
}

static __inline__ uint32_t t_set_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r ^ v) ^ r -> r
	// r = 1, v = 1 : (1 ^ 1) ^ 1  = 1   // v=1, r set
	// r = 0, v = 1 : (0 ^ 1) ^ 0  = 1   // v=1, r set
	// r = 1, v = 0 : (1 ^ 0) ^ 1  = 0   // v=0, r not changed
	// r = 0, v = 0 : (0 ^ 0) ^ 0  = 0   // v=0, r not changed
	//printf("value= 0x%08X, t_register=0x%08X\n",value, x_value);
	value = value & t_mask;
	// (reg OR val) is a required value of 'reg' after hw-XOR-ing a looked for number
	// i.e. we need to make a preceeding XOR to get a number which will be XOR-ed with 'reg' by hardware
	// (t_register | value) ^ t_register  ^(by hw)  t_register  == (t_register | value)
	uint32_t x_value = (t_register | value) ^ (t_register & t_mask);
	//printf("t_register | value =0x%08X, t_register & t_mask = 0x%08X, x_value = 0x%08X\n", (t_register | value),\
(t_register & t_mask), x_value);
	return x_value;
}

static __inline__ uint32_t t_clear_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	//                (r & v) ^ r -> r
	// r = 1, v = 0 : (1 & 0) ^ 1  = 1   // v=1, r not changed
	// r = 0, v = 0 : (0 & 0) ^ 0  = 1   // v=1, r not changed
	// r = 1, v = 1 : (1 & 1) ^ 1  = 0   // v=0, r cleared
	// r = 0, v = 1 : (0 & 1) ^ 0  = 0   // v=0, r cleared

	// if m == 0 and either v == 0 or v == 1, we don't touch r
	// if m == 1 and v == 1, we reset r.
	// For example we want to reset bits using value=0b1010 and mask=0b1100. That means
	// our masked 'value' is going to be 0b1000.
	// For clearing a bit we use bitwise-AND between r and 1 (one) preceeding hardware bitwise-XOR,
	// i.e (r & 1) == r ----> (this xor is made by usb-core) r ^ r = 0
	value = value & t_mask;
	uint32_t x_value = (t_register & value) | (t_register & ~t_mask);
	return x_value;
}

static __inline__ uint32_t set_bits(uint32_t value, uint32_t reg, uint32_t w_mask, uint32_t w0_mask, uint32_t t_mask)
{
	uint32_t w_value = value & w_mask;
	uint32_t t_value = value & t_mask;

	uint32_t new_register = w_set_bits(w_value, reg, w_mask);
	//printf("new_reg =0x%08X\n", new_register);

	uint32_t x_value = t_set_bits(t_value, new_register, t_mask);
	//printf("t_new_reg=0x%08X\n", x_value);

	return x_value;
}

static __inline__ uint32_t clear_bits(uint32_t value, uint32_t reg, uint32_t w_mask, uint32_t w0_mask, uint32_t t_mask)
{
	uint32_t w_value = value & (w_mask | w0_mask);
	uint32_t t_value = value & t_mask;

	uint32_t new_register = w_clear_bits(w_value, reg, (w_mask | w0_mask));
	//printf("new_reg =0x%08X\n", new_register);

	uint32_t x_value = t_clear_bits(t_value, new_register, t_mask);
	//printf("t_new_reg=0x%08X\n", x_value);

	return x_value;
}


#endif
