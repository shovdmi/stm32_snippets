#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//#include "ep.h"
//#include "btable.h"
#include "pma.h"

void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
#ifdef TEST_ON_TARGET
#endif
}

void tearDown(void)
{
}


enum __attribute__ ((__packed__)) bmRequestType_type_t {
  REQUIEST_STANDARD      = 0,
  REQUIEST_CLASS         = 1,
  REQUIEST_VENDOR        = 2,
  REQUIEST_RESERVED_TYPE = 3,
};
enum __attribute__ ((__packed__)) bmRequestType_recipient_t {
  RECIPIENT_STANDARD = 0,
  RECIPIENT_CLASS    = 1,
  RECIPIENT_VENDOR   = 2,
  RECIPIENT_OTHER    = 3,
  RECIPIENT_RESERVED_TYPE = 4, // 4..31 = Reserved
};

enum __attribute__ ((__packed__)) bmRequestType_direction_t {
  DIRECTION_OUT = 0,
  DIRECTION_IN  = 1,
};

  enum usb_standard_requests_t {
    GET_STATUS     = 0,
    CLEAR_FEATURE  = 1,

    SET_FEATURE    = 3,

    SET_ADDRESS    = 5,
    GET_DESCRIPTOR = 6,
    SET_DESCRIPTOR = 7,
    GET_CONFIGURATION = 8,
    SET_CONFIGURATION = 9,
    GET_INTERFACE  = 10,
    SET_INTERFACE  = 11,
    SYNCH_FRAME    = 12,
  };

  // Endopoint 0 received SETUP PID
  {
    switch (setup_pma->bRequest)
    {
    case GET_STATUS:     // 0x00
      __asm__("bkpt");
      break;

    case CLEAR_FEATURE:  // 0x01
      // Not implemented yet. For now respond ZPL only
      //send_IN_stall_ep0();
      send_ZLP(ep);
      //flag = 4;
      break;

    case SET_FEATURE:  // 0x03
      // Not implemented yet. For now respond ZPL only
      //send_IN_stall_ep0();
      send_ZLP(ep);
      break;

    case SET_ADDRESS:  // 0x05
      send_ZLP(ep);
      USB->DADDR = USB_DADDR_EF | setup_pma->wValue;
      break;

    case GET_DESCRIPTOR:// 0x06
      switch (setup_pma->bmRequestType.recipient)
      {
      case DEVICE: // Device get descriptor // 0x0680 0x0100 0x0000 0x0012 (packet: 80 06 ........)
        device_get_descriptor(ep , setup_pma);
        break;

      case INTERFACE: // Interface get descriptor // 0x0681 0x2200 0x0000 0x0057 (packet: 81 06 .......)
        interface_get_descriptor(ep, setup_pma);
        break;

      case ENDPOINT: // Endpoint get descriptor
        __asm__("bkpt");
        break;

      case OTHER: // Other get descriptor
        __asm__("bkpt");
        break;

      default: // 4-31 = Reserved
        __asm__("bkpt");
        break;
      }
      break;

    case SET_DESCRIPTOR:// 0x07
      __asm__("bkpt");
      break;

    case GET_CONFIGURATION:  // 0x08
      __asm__("bkpt");
      break;

    case SET_CONFIGURATION:  // 0x09
      // Not implemented yet. For now respond ZPL only
      //send_IN_stall_ep0();
      send_ZLP(ep);
      break;

    case GET_INTERFACE:  // 0x0A
      __asm__("bkpt");
      break;

    case SET_INTERFACE:  // 0x0B
      __asm__("bkpt");
      break;

    case SYNCH_FRAME:  // 0x0C
      __asm__("bkpt");
      break;

    default:
      __asm__("bkpt");
      break;
    }
  }
}

static inline bmRequestType_type_t get_request_type(uint8_t bmRequestType)
{
  bmRequestType_type_t type = (bmRequestType >> 5) & 0x03;
  return type;
}

static inline bmRequestType_type_t get_request_recipient(uint8_t bmRequestType)
{
  bmRequestType_recipient_t recipient = bmRequestType & 0x0F;
  return recipient;
}

static inline bmRequestType_type_t get_request_direction(uint8_t bmRequestType)
{
  bmRequestType_direction_t direction = (bmRequestType >> 7) & 0x01;
  return direction;
}

int parse_packet(size_t pma_offset, size_t len)
{
  uint8_t bmRequestType = pma_read_u8(pma_offset);
  
  return 0;
}

void test_parse_packet(void)
{
  printf("0x%02X ", read_pma_u8(btable_ep[0].rx_addr + i));
}
