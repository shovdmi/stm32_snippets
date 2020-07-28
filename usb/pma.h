#ifndef PMA_H
#define PMA_H

uint16_t read_pma_u16_aligned(size_t offset);

uint8_t read_pma_u8(size_t offset);

uint16_t read_pma_u16(size_t offset);

void read_from_pma(unsigned int pma_bytes_offset, uint8_t *dest_buf, int len);

#endif // PMA_H