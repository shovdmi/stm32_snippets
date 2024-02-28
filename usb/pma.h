#ifndef PMA_H
#define PMA_H

#include "stdint.h"
#include "string.h"

/* 
	 PMA_32BIT_ACCESS
      Firmware point of view                 USB dedicated hardware point of view
+-------------+-------------+----------+        +-------------+-------------+
| MCU address | PMA address |uint32_t* |        | PMA address | MCU address |
+--------------------------------------+        +---------------------------+
| 0x4000 6000 |   0x0000    |   0x0    |        |   0x0000    | 0x4000 6000 |
+---------------------------+          |        +---------------------------+
| 0x4000 6001 |   0x0001    |          |        |   0x0001    | 0x4000 6001 |  <-- unaligned access
+---------------------------+          |        +---------------------------+
| 0x4000 6002 |             |          |        |   0x0002    | 0x4000 6004 | <-- !! 6004
+---------------------------+          |        +---------------------------+
| 0x4000 6003 |             |          |        |   0x0003    | 0x4000 6005 | <-- unaligned access
+--------------------------------------+        +---------------------------+
| 0x4000 6004 |   0x0002    |   0x1    |        |   0x0004    | 0x4000 6008 | <-- !! 6008
+---------------------------+          |        +---------------------------+
| 0x4000 6005 |   0x0003    |          |        |   0x0005    | 0x4000 6009 | <-- unaligned access
+---------------------------+          |        +-------------+-------------+
| 0x4000 6006 |             |          |
+---------------------------+          |
| 0x4000 6007 |             |          |
+--------------------------------------+
| 0x4000 6008 |   0x0004    |   0x2    |
+---------------------------+          |
| 0x4000 6009 |   0x0005    |          |
+---------------------------+          |
| 0x4000 6010 |             |          |
+---------------------------+          |
| 0x4000 6011 |             |          |
+-------------+-------------+----------+

	
	 PMA_16BIT_ACCESS
      Firmware point of view                 USB dedicated hardware point of view
+-------------+-------------+----------+        +-------------+-------------+
| MCU address | PMA address |uint16_t* |        | PMA address | MCU address |
+--------------------------------------+        +---------------------------+
| 0x4000 6000 |   0x0000    |   0x0    |        |   0x0000    | 0x4000 6000 |
+---------------------------+          |        +---------------------------+
| 0x4000 6001 |   0x0001    |          |        |   0x0001    | 0x4000 6001 | <-- unaligned access
+---------------------------+----------+        +---------------------------+
| 0x4000 6002 |   0x0002    |   0x1    |        |   0x0002    | 0x4000 6002 |
+---------------------------+          |        +---------------------------+
| 0x4000 6003 |   0x0003    |          |        |   0x0003    | 0x4000 6003 | <--
+--------------------------------------+        +---------------------------+
| 0x4000 6004 |   0x0004    |   0x2    |        |   0x0004    | 0x4000 6004 |
+---------------------------+          |        +---------------------------+
| 0x4000 6005 |   0x0005    |          |        |   0x0005    | 0x4000 6005 | <--
+---------------------------+----------+        +-------------+-------------+
| 0x4000 6006 |   0x0006    |   0x03   |
+---------------------------+          |
| 0x4000 6007 |   0x0007    |          |
+--------------------------------------+
| 0x4000 6008 |   0x0008    |   0x4    |
+---------------------------+          |
| 0x4000 6009 |   0x0009    |          |
+---------------------------+----------+
| 0x4000 6010 |   0x0010    |   0x05   |
+---------------------------+          |
| 0x4000 6011 |   0x0011    |          |
+-------------+-------------+----------+

*/

#ifndef PMA_ADDRESS
  #define PMA_ADDRESS ((uintptr_t)0x40006000)
#endif

// https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html#Common-Variable-Attributes
// https://gcc.gnu.org/onlinedocs/gccint/Machine-Modes.html
// https://gist.github.com/maxymania/b14c617af5063a21d2b1c7e84d024c74#file-gcc-typedefs-c

#if defined STM32F1
// SImode “Single Integer” mode represents a four-byte integer.
typedef uint16_t __attribute__((mode(SI))) pma_uint16_t;
#elif defined STM32L0
typedef uint16_t pma_uint16_t;
#else
#error Representing mode is undefined!
#endif


uint16_t read_pma_u16_aligned(size_t offset);

uint8_t read_pma_u8(size_t offset);

uint16_t read_pma_u16(size_t offset);

void read_from_pma(size_t pma_bytes_offset, uint8_t *dest_u8_buf, size_t len);

void read_from_pma_slow(size_t offset, uint8_t *dest_u8_buf, size_t len);

void read_pma_aligned(size_t offset, uint16_t *dest_u16_buf, size_t len);

void read_pma(size_t offset,       uint8_t *dest_u8_buf, size_t len);
void write_pma(size_t offset, const uint8_t *src_buf, size_t len);

#endif // PMA_H
