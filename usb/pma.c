#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef TEST
//#define PMA_ADDRESS ((uintptr_t)0x40006000)
uint16_t pool[128];
#else
extern uint16_t pool[128];
#define PMA_ADDRESS ((uint8_t*)&pool[0])
#endif


// https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#Common-Variable-Attributes
// https://gcc.gnu.org/onlinedocs/gccint/Machine-Modes.html
// https://gist.github.com/maxymania/b14c617af5063a21d2b1c7e84d024c74#file-gcc-typedefs-c

typedef uint16_t __attribute__((mode(SI))) pma_uint16_t;

uint16_t read_pma_u16_aligned(size_t offset)
{
  pma_uint16_t *addr = ((pma_uint16_t*)(PMA_ADDRESS + (offset << 1)));
  pma_uint16_t val = *addr;
  return val;
}

//-----------------------------------------------------------------------------------------------

uint8_t read_pma_u8(size_t offset)
{
	pma_uint16_t val;

	// int type of last_bit is used as bool below
	size_t offset_lsb = offset & 0x01U;
	if (offset_lsb == 0x01U)
	{
	// unaligned access
		offset = offset - offset_lsb; // clear LSB
		val = read_pma_u16_aligned(offset);
		val = val >> 8;
	}
	else
		val = read_pma_u16_aligned(offset);

  return val;
}

uint16_t read_pma_u16(size_t offset)
{
	int offset_lsb = offset & 0x01U;

	if (offset_lsb == 0x01U)
	{
		// unaligned access
		offset = offset - offset_lsb;
		pma_uint16_t val_l = read_pma_u16_aligned(offset);
		pma_uint16_t val_h = read_pma_u16_aligned(offset + sizeof(uint16_t));
		return  (val_l >> 8) | ((val_h & 0xFFU) << 8);
	}
	else
		return read_pma_u16_aligned(offset);
}


void read_from_pma(unsigned int pma_bytes_offset, uint8_t *dest_buf, int len)
{
	uint8_t *pma_src_buf = (uint8_t *)PMA_ADDRESS + pma_bytes_offset;
	for (int i = 0; i<len/2; i++)
	{
		((uint16_t*)dest_buf)[i] = (uint16_t)(((uint16_t*)pma_src_buf)[i]);
	}
	if ((len & 0x01) == 0x01) { // i.e. len is odd
		((uint8_t*)dest_buf)[len-1] = (uint8_t)(((uint16_t*)pma_src_buf)[len/2]);
	}
}

/*void read_from_pma_new(size_t offset, void *dest_buf, size_t len)
{
	if ((offset & 0x01) == 0x01) {
		((uint8_t*)dest_buf[0] = read_pma_u8(offset);
	}
	for (int i = 0; i < len / 2; i += 2)
	{
		(uint16_t *)dest_buf[i] = read_pma_u16_aligned(i);
	}
	if (((offset+len) & 0x01) == 0x01) { // i.e. either len or offset is odd
		((uint8_t*)dest_buf[len-1] = read_pma_u8(len/2);
	}
}*/

#if 0
int main(void)
{
  for(int i = 0; i < sizeof(pool); i++) {
    *((uint8_t*)(&pool[0])+i) = (i % 255);
  }

  for (int i= 0; i < sizeof(pool) / 2; i+=2) {
//    printf("pool[%d]: %d\n", i, read_pma_u8(i));
  }

  for (int i= 0; i < 64; i++) {
    printf("pma[%d]: 0x%04X\n", i, read_pma_u16(i));
    //printf("pma[%d]: 0x%04X\n", i, read_pma_u8(i));
  }
  printf("sizeof(pma_uint16_t)=%ld\n", sizeof(pma_uint16_t));
  return 0;
}
#endif
