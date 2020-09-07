#ifndef BITS_OPERATION_H
#define BITS_OPERATION_H

extern uint32_t w_write(uint32_t value, uint32_t rw_register, uint32_t rw_mask);
extern uint32_t w_set(uint32_t value, uint32_t rw_register, uint32_t rw_mask);
extern uint32_t w_clear(uint32_t value, uint32_t rw_register, uint32_t rw_mask);
extern uint32_t w0_write(uint32_t value, uint32_t w0_register, uint32_t w0_mask);
extern uint32_t t_write(uint32_t value, uint32_t t_register, uint32_t t_mask);
extern uint32_t t_set_bits(uint32_t value, uint32_t t_register, uint32_t t_mask);
extern uint32_t t_clear_bits(uint32_t value, uint32_t t_register, uint32_t t_mask);

#endif
