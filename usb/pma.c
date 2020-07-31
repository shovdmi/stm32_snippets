#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pma.h"


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
		((uint16_t*)dest_buf)[i] = (uint16_t)(((uint32_t*)pma_src_buf)[i]);
	}
	if ((len & 0x01) == 0x01) { // i.e. len is odd
		((uint8_t*)dest_buf)[len-1] = (uint8_t)(((uint32_t*)pma_src_buf)[len/2]);
	}
}

void read_from_pma_slow(size_t offset, void *dest_buf, size_t len)
{
	for (int i = 0; i < len; i++ )
	{
		((uint8_t *)dest_buf)[i] = read_pma_u8(offset + i);
	}
}

void read_pma_aligned(size_t offset, void *dest_buf, size_t len)
{
	size_t i = offset;
	size_t j = 0;
	for ( ; i < offset + len; i+=sizeof(uint16_t), j++)
	{
		((uint16_t*)dest_buf)[j] = read_pma_u16_aligned(i);
	}
}
