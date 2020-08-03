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

void read_pma_aligned(size_t offset, void *dest_buf, size_t length)
{
	for (size_t i = offset, j = 0 ; i < offset + length; i+=sizeof(uint16_t), j++)
	{
		((uint16_t*)dest_buf)[j] = read_pma_u16_aligned(i);
	}
}

void read_pma(size_t offset, void *dest_buf, size_t length)
{
	size_t offset_lsb = offset & 0x01U;

	if (offset_lsb == 0x01U)
	{
		((uint8_t*)dest_buf)[0] = read_pma_u8(offset);
		dest_buf = ((uint8_t*)dest_buf) + 1;
		length--;
	}

	// Here we increase offset to be aligned to 16bit
	size_t offset_aligned = offset + offset_lsb;

	size_t length_lsb = length & 0x01U;
	size_t length_aligned = length - length_lsb;

	read_pma_aligned(offset_aligned, dest_buf, length_aligned);

	if (length_lsb == 0x01U)
	{
		((uint8_t*)dest_buf)[length-1] = read_pma_u8(offset_aligned + length - 1);
	}
}
