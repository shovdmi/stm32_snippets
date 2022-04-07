#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <stdint.h>

/** \breif
 * Here we assume the register has no toggleble bits. If it has then tbw-value must be bitwise AND-ed with (~t_mask).
 * [bit_ops.h](:/6bfee0f41b334992b47d027522aa9b81)
 * @param value
 * @param rw_register - register current value
 * @param rw_mask - (register writeble bit mask / rw_mask) : 1 - read/write bit; 0 - keep (write back) bit current value
 * @return
 */
static __inline__ uint32_t w_write_bits(uint32_t value, uint32_t v_mask, uint32_t rw_register, uint32_t rw_mask)
{
	// A: rw_register; B: value; C: rw_mask; D: v_mask;
	uint32_t result = (rw_register | rw_mask) & (rw_register | v_mask) & ( value | (~rw_mask) | (~v_mask));
	return result;
}

static __inline__ uint32_t w_modify_bits(uint32_t value, uint32_t v_mask, uint32_t rw_register)
{
	uint32_t result = w_write_bits(value, v_mask, rw_register, 0xFFFFFFFF);
	return result;
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

static __inline__ uint32_t w_toggle_bits(uint32_t value, uint32_t rw_register, uint32_t rw_mask)
{
	value = value & rw_mask;
	rw_register = rw_register ^ value;
	return rw_register;
}

/** \brief 
 * Here we assume the register has no toggleble bits. If it has then tbw-value must be bitwise AND-ed with (~t_mask).
 * [bit_ops.h](:/6bfee0f41b334992b47d027522aa9b81)
 * @param value
 * @param w0_register - register current value
 * @param w0_mask - (register write-zero bit mask / w0_mask) : 1 - write-0 to reset bit; 0 - keep (write back) bit current value
 * @return
 */
static __inline__ uint32_t w0_write_bits(uint32_t value, uint32_t v_mask, uint32_t w0_register, uint32_t w0_mask)
{
	uint32_t result = w0_register & ( value | (~w0_mask) | (~v_mask));
	return result;
}

/** \brief
 */
static __inline__ uint32_t t_write_bits(uint32_t value, uint32_t v_mask, uint32_t t_register, uint32_t t_mask)
{
	uint32_t result = (t_register & (~t_mask)) | (t_register & (~value) & v_mask) | ((~t_register) & value & t_mask & v_mask);
	return result;
}

static __inline__ uint32_t t_set_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	// see [bit_ops.h](:/6bfee0f41b334992b47d027522aa9b81)
	//printf("value= 0x%08X, t_register=0x%08X\n",value, x_value);
	value = value & t_mask;
	uint32_t x_value = (t_register | value) ^ (t_register & t_mask);
	//printf("t_register | value =0x%08X, t_register & t_mask = 0x%08X, x_value = 0x%08X\n", (t_register | value), (t_register & t_mask), x_value);
	return x_value;
}

static __inline__ uint32_t t_clear_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	// see [bit_ops.h](:/6bfee0f41b334992b47d027522aa9b81)
	value = value & t_mask;
	uint32_t x_value = (t_register & value) | (t_register & (~t_mask));
	return x_value;
}

static __inline__ uint32_t t_toggle_bits(uint32_t value, uint32_t t_register, uint32_t t_mask)
{
	value = value & t_mask;
	uint32_t x_value = (t_register &(~t_mask)) | value;
	return x_value;
}

static __inline__ uint32_t set_bits(uint32_t value, uint32_t reg, uint32_t w_mask, uint32_t w0_mask, uint32_t t_mask)
{
	(void)w0_mask;
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

static __inline__ uint32_t toggle_bits(uint32_t value, uint32_t reg, uint32_t w_mask, uint32_t w0_mask, uint32_t t_mask)
{
	uint32_t w_value = value & (w_mask | w0_mask);
	uint32_t t_value = value & t_mask;

	uint32_t new_register = w_toggle_bits(w_value, reg, (w_mask | w0_mask));
	//printf("new_reg =0x%08X\n", new_register);

	uint32_t x_value = t_toggle_bits(t_value, new_register, t_mask);
	//printf("t_new_reg=0x%08X\n", x_value);

	return x_value;
}

static __inline__ uint32_t write_bits(uint32_t value, uint32_t v_mask, uint32_t reg, uint32_t w_mask, uint32_t w0_mask, uint32_t t_mask)
{
	uint32_t w_value = value & w_mask;
	uint32_t new_register = w_write_bits(w_value, v_mask, reg, w_mask);

	uint32_t w0_value = value & w0_mask;
	new_register = w0_write_bits(w0_value, v_mask, new_register, w0_mask);

	uint32_t t_value = value & t_mask;

	uint32_t x_value = t_write_bits(t_value, v_mask, new_register, t_mask);

	return x_value;
}

#endif
