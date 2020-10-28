#ifndef BITS_OPERATION_H
#define BITS_OPERATION_H

#include <stdint.h>

/** \breif
 *
 * R    | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  A
 * V    | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |  B
 * WM   | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  C
 * VM   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  D
 * -----|---|---|---|---|---|---|---|---|----|---|---|---|---|---|---|---|
 * TBW  | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 |  Y
 *
 * A: rw_register; B: value; C: rw_mask; D: v_mask;
 *
 * y = (A + C) (A + D) (B + C' + D')
 * http://www.32x8.com/pos4_____A-B-C-D_____m_7-8-9-10-12-13-14-15___________option-2_____899788975271824292797
 *
 * y = AC' + AD' + BCD // ?? == A(C'+D') + BCD = A((CD)') + BCD = (A' + CD)' + BCD
 * http://www.32x8.com/sop4_____A-B-C-D_____m_7-8-9-10-12-13-14-15___________option-0_____899788975271824292797
 *
 *
 * @param value
 * @param rw_register
 * @param rw_mask
 * @return
 */
static __inline__ uint32_t w_write_bits(uint32_t value, uint32_t v_mask, uint32_t rw_register, uint32_t rw_mask)
{
	// A: rw_register; B: value; C: rw_mask; D: v_mask;
	uint32_t result = (rw_register | rw_mask) & (rw_register | v_mask) & ( value | ~rw_mask | ~v_mask);
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
 * R - register value; V - expected register value; tbw- to be written
 *
 * | R | V || tbw | | R op V |
 * |---|---||-----| |--------|
 * | 0 | 0 ||  0  | | = 0&0  |
 * | 0 | 1 ||  0  | | = 0&1  | writing 1's keeps R
 * | 1 | 0 ||  0  | | = 1&0  |
 * | 1 | 1 ||  1  | | = 1&1  | writing 1's keeps R
 *
 * R    | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  A
 * V    | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |  B
 * W0M  | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  C
 * VM   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  1 | 1 | 1 | 1 | 1 | 1 | 1 | 1 |  D
 * -----|---|---|---|---|---|---|---|---|----|---|---|---|---|---|---|---|
 * TBW  | 0 | x | 0 | x | 1 | 1 | 1 | 1 |  0 | x | 0 | x | 1 | 0 | 1 | 1 |  Y
 *
 * A: rw_register; B: value; C: w0_mask; D: v_mask;
 *
 * y = AC' + AD' + AB
 * y = (A) (B + C' + D')
 *
 *http://www.32x8.com/pos4_____A-B-C-D_____m_8-9-10-12-13-14-15_____d_2-3-6-7_____option-2_____889788965478820592664
 */
static __inline__ uint32_t w0_write_bits(uint32_t value, uint32_t v_mask, uint32_t w0_register, uint32_t w0_mask)
{
	uint32_t result = w0_register & ( value | ~w0_mask | ~v_mask);
	return result;
}

/** \brief
 * R - register value; V - expected register value; tbw- to be written
 *
 * | R | V || tbw | | R op V |
 * |---|---||-----| |--------|
 * | 0 | 0 ||  0  | | = 0^0  | writing 0's keeps R
 * | 0 | 1 ||  1  | | = 0^1  |
 * | 1 | 0 ||  1  | | = 1^0  |
 * | 1 | 1 ||  0  | | = 1^1  | writing 0's keeps R
 *
 * R    | 0 | 0 | 0 | 0 | 1 | 1 | 1 | 1 |  0 | 0 | 0 | 0 | 1 | 1 | 1 | 1  |  A
 * V    | 0 | 0 | 1 | 1 | 0 | 0 | 1 | 1 |  0 | 0 | 1 | 1 | 0 | 0 | 1 | 1  |  B
 * TM   | 0 | 1 | 0 | 1 | 0 | 1 | 0 | 1 |  0 | 1 | 0 | 1 | 0 | 1 | 0 | 1  |  C
 * VM   | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |  1 | 1 | 1 | 1 | 1 | 1 | 1 | 1  |  D
 * -----|---|---|---|---|---|---|---|---|----|---|---|---|---|---|---|----|
 * TBW  | 0 | 0 | 0 | 0 | 1 | 0 | 1 | 0 |  0 | 0 | 0 | 1~| 1 | 1~| 1 | 0  |  Y
 *        r       r       r       r        r       r       r       r
 *
 * A: rw_register; B: value; C: t_mask; D: v_mask;
 *
 * y = y = AC' + AB'D + A'BCD
 * y = y = (A + B) (A + C) (C' + D) (A' + B' + C')
 *
 * http://www.32x8.com/sop4_____A-B-C-D_____m_7-8-10-11-12-13___________option-0_____898788876971825592647
 */
static __inline__ uint32_t t_write_bits(uint32_t value, uint32_t v_mask, uint32_t t_register, uint32_t t_mask)
{
	uint32_t result = t_register & ~t_mask | t_register & ~value & v_mask | ~t_register & value & t_mask & v_mask;
	return result;
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
	// i.e. we need to make a preceding XOR to get a number which will be XOR-ed with 'reg' by hardware
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
	// For clearing a bit we use bitwise-AND between r and 1 (one) preceding hardware bitwise-XOR,
	// i.e (r & 1) == r ----> (this xor is made by usb-core) r ^ r = 0
	value = value & t_mask;
	uint32_t x_value = (t_register & value) | (t_register & ~t_mask);
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
