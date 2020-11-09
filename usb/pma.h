#ifndef PMA_H
#define PMA_H

#include "stm32f1xx.h"

#ifdef TEST_ON_TARGET
    #define PMA_ADDRESS ((uintptr_t)0x40006000)
#else
  #ifdef TEST
	extern uint8_t pool[];
    	#define PMA_ADDRESS (&pool[0])
  #endif
#endif


// https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#Common-Variable-Attributes
// https://gcc.gnu.org/onlinedocs/gccint/Machine-Modes.html
// https://gist.github.com/maxymania/b14c617af5063a21d2b1c7e84d024c74#file-gcc-typedefs-c

// SImode “Single Integer” mode represents a four-byte integer.
typedef uint16_t __attribute__((mode(SI))) pma_uint16_t;


uint16_t read_pma_u16_aligned(size_t offset);

uint8_t read_pma_u8(size_t offset);

uint16_t read_pma_u16(size_t offset);

void read_from_pma(size_t pma_bytes_offset, uint8_t *dest_buf, size_t len);

void read_from_pma_slow(size_t offset, void *dest_buf, size_t len);

void read_pma_aligned(size_t offset, void *dest_buf, size_t len);

void read_pma(size_t offset, void *dest_buf, size_t len);

#endif // PMA_H
