#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pma.h"

#ifndef ASSERT
#define ASSERT(...)
#endif


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

void write_pma_u16_aligned(size_t offset, uint16_t value)
{
	pma_uint16_t *addr = (pma_uint16_t*)PMA_ADDRESS + (offset / (sizeof (uint16_t)));
    *addr = value;
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

void write_pma_u8(size_t offset, uint8_t value)
{
	size_t offset_lsb = offset & 0x01U;
    size_t offset_aligned = offset - offset_lsb;

    uint16_t pma_val = read_pma_u16_aligned(offset_aligned);
    
    if (offset_lsb == 0x01U) {
        pma_val &= 0xFF00 | value;
    }
    else {
        pma_val &= 0x00FF | (value << 8);
    }
    write_pma_u16_aligned(offset_aligned, pma_val);
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
	/*for (size_t i = offset, j = 0 ; i < offset + length; i+=sizeof(uint16_t), j++)
	{
		dest_u16_buf[j] = read_pma_u16_aligned(i);
	} */

	size_t i = offset;
	size_t j = 0;
	while (i < offset + length) {
		dest_u16_buf[j] = read_pma_u16_aligned(i);
		 i+=sizeof(uint16_t); 
		j++;
	}
}

void write_pma_aligned(size_t offset, const uint16_t *src_buf, size_t length)
{
    // Iterate over two uint8_t (i.e. uint16_t) elements
    for (int i = 0; i < length; ++i) {
        const uint16_t value = src_buf[i];
        write_pma_u16_aligned(offset + i * sizeof(uint16_t), value);
    }
}

void read_pma(size_t offset, uint8_t *dest_u8_buf, size_t length)
{
	ASSERT(length > 0);
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

	// Reading from aligned offset of PMA data
    ASSERT(((unsigned)dest_u8_buf & ~0x01U) == (unsigned)dest_u8_buf); // Comment this out when you are sure the MCU supports unaligned or one-byte reading accesses 
	uint16_t *u16_buf = (uint16_t*)dest_u8_buf;
	read_pma_aligned(offset_aligned, u16_buf, length_aligned);

	if (length_lsb == 0x01U)
	{
		dest_u8_buf[length-1] = read_pma_u8(offset_aligned + length - 1);
	}
}

void write_pma(size_t pma_offset, const uint8_t *src_buf, size_t length)
{
    size_t offset_lsb = pma_offset & 0x01U;

	if (offset_lsb == 0x01U)
	{
    	write_pma_u8(pma_offset, src_buf[0]);
		src_buf++;
		length--;
	}

	// Here we increase offset to be aligned to 16bit
    size_t offset_aligned = pma_offset + offset_lsb;

	size_t length_lsb = length & 0x01U;
	size_t length_aligned = length - length_lsb;

	// Writing at an aligned offset of the PMA data
    ASSERT(((unsigned)src_buf & ~0x01U) == (unsigned)src_buf); // Comment this out when you are sure the MCU supports unaligned or one-byte reading accesses 
	uint16_t *u16_buf = (uint16_t*)src_buf;
    write_pma_aligned(offset_aligned, u16_buf, length_aligned / sizeof(uint16_t)); 

	// Write the last byte, if any
    if (length_lsb == 0x01U)
    {
        write_pma_u8(offset_aligned + length - 1, src_buf[length - 1]);
    }
}
