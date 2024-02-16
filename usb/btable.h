#ifndef BTABLE_H
#define BTABLE_H

#include "pma.h"

//	1) C99 §6.7.2.1, paragraph 10 says: "The order of allocation of bit-fields within a unit (high-order to low-order or low-order to high-order) is implementation-defined."
//
//	2) https://gcc.gnu.org/onlinedocs/gcc/Structures-unions-enumerations-and-bit-fields-implementation.html
//
//	3) struct iphdr {
//	#if defined(__LITTLE_ENDIAN_BITFIELD)
//	        __u8    ihl:4,
//	                version:4;
//	#elif defined (__BIG_ENDIAN_BITFIELD)
//	        __u8    version:4,
//	                ihl:4;
//	#else
//	#error  "Please fix <asm/byteorder.h>"
//	#endif
//
// 4) Be carefull! 
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0491i/Babjddhe.html " You can explicitly pad a bitfield container by declaring an unnamed bitfield of size zero. A bitfield of zero size fills the container up to the end if the container is not empty. A subsequent bitfield declaration starts a new empty container. "
//
//	5) ARM already has a bitfield operation BFI, BCI such as "bfi w0, w1, 2, 5" 
//
union USB_COUNTn_RX {
  struct /*__attribute__((packed)) */{
    pma_uint16_t COUNT_RX    : 10;
    pma_uint16_t NUM_BLOCK   :  5;
    pma_uint16_t BL_SIZE_TYPE:  1;//  If BL_SIZE=0, the memory block is 2 byte large. The allocated buffer size ranges from 2 to 62 bytes.
                                  //  If BL_SIZE=1, the memory block is 32 byte large. The allocated buffer size ranges from 32 to 1024 bytes
  }; // TODO: read about gcc flag for bits order in bit-struct
  pma_uint16_t value;
};

struct btable_ep_t {
  pma_uint16_t tx_addr;
  pma_uint16_t tx_count;
  pma_uint16_t rx_addr;
  union USB_COUNTn_RX rx_count;
};

#endif // BTABLE_H
