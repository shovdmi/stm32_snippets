#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pma.h"


/**
 *
 * offset -- offset in bytes (in terms of PMA bytes)
 * PMA:   00 01        04 05        08 09        0C 0D
 * offset: 0  1         2  3         ^--offset=4
 */
uint16_t read_pma_u16_aligned(size_t offset)
{
	pma_uint16_t *addr = (pma_uint16_t*)PMA_ADDRESS + (offset / (sizeof (uint16_t)));
	pma_uint16_t val = *addr;
	return val;
}

//-----------------------------------------------------------------------------------------------

uint8_t read_pma_u8(size_t offset)
{
	pma_uint16_t val;

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
	size_t offset_lsb = offset & 0x01U;

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


void read_from_pma(size_t pma_bytes_offset, uint8_t *dest_u8_buf, size_t len)
{
	uint8_t *pma_src_buf = (uint8_t *)PMA_ADDRESS + pma_bytes_offset;
	for (size_t i = 0; i<len/2; i++)
	{
		((uint16_t*)dest_u8_buf)[i] = (uint16_t)(((uint32_t*)pma_src_buf)[i]);
	}
	if ((len & 0x01) == 0x01) { // i.e. len is odd
		dest_u8_buf[len-1] = (uint8_t)(((uint32_t*)pma_src_buf)[len/2]);
	}
}

void read_from_pma_slow(size_t offset, uint8_t *dest_u8_buf, size_t len)
{
	for (size_t i = 0; i < len; i++ )
	{
		dest_u8_buf[i] = read_pma_u8(offset + i);
	}
}

void read_pma_aligned(size_t offset, uint16_t *dest_u16_buf, size_t length)
{
	for (size_t i = offset, j = 0 ; i < offset + length; i+=sizeof(uint16_t), j++)
	{
		dest_u16_buf[j] = read_pma_u16_aligned(i);
	}
}

void read_pma(size_t offset, uint8_t *dest_u8_buf, size_t length)
{
	//assert(length > 0);
	if (length == 0)
		return;
	
	size_t offset_lsb = offset & 0x01U;

	if (offset_lsb == 0x01U)
	{
		dest_u8_buf[0] = read_pma_u8(offset);
		dest_u8_buf++;
		length--;
	}

	// Here we increase offset to be aligned to 16bit
	size_t offset_aligned = offset + offset_lsb;

	size_t length_lsb = length & 0x01U;
	size_t length_aligned = length - length_lsb;

	uint16_t *u16_buf = (uint16_t*)dest_u8_buf;
	read_pma_aligned(offset_aligned, u16_buf, length_aligned);

	if (length_lsb == 0x01U)
	{
		dest_u8_buf[length-1] = read_pma_u8(offset_aligned + length - 1);
	}
}



void write_to_pma(const uint8_t *src_buf, uint16_t pma_bytes_offset, int N)
{
    //assert((pma_bytes_offset & (0xFFFFu << 1)) != pma_bytes_offset);
    if ((pma_bytes_offset & (0xFFFFu << 1)) != pma_bytes_offset) { __asm__("bkpt"); }
    //assert(pma_bytes_offset >= 512);
    //assert(pma_bytes_offset + N > 512);
	
    pma_uint16_t *dest_buf = (pma_uint16_t*)PMA_ADDRESS + (pma_bytes_offset / (sizeof(uint16_t)));

    for (int i = 0; i < (N + 1) / 2; i++) 
    {
        ((pma_uint16_t*)dest_buf)[i]  = ((uint16_t*)src_buf)[i];
    }	
}
