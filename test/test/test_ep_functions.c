#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ep.h"
#include "btable.h"
#include "pma.h"


void Clock_Configure(void)
{
	 // Enable Prefetch Buffer 
	 FLASH->ACR |= FLASH_ACR_PRFTBE;

	 // Flash 2 wait state
	 FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
	 FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

	 // Clock Configuration
		RCC->CR |= RCC_CR_HSEON; // 19.1
		while ((RCC->CR & RCC_CR_HSERDY) == 0) { ; }

		RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;	// 0: HSE clock not divided // 19.2

		RCC->CFGR |= RCC_CFGR_PLLSRC;     // 1: HSE oscillator clock selected as PLL input clock // 19.3

		RCC->CR &= ~RCC_CR_PLLON;         // 19.4
		RCC->CFGR &= ~RCC_CFGR_PLLMULL;
		RCC->CFGR |=	RCC_CFGR_PLLMULL_0 | RCC_CFGR_PLLMULL_1 | RCC_CFGR_PLLMULL_2; // 0111: x9

		RCC->CFGR &= ~RCC_CFGR_HPRE;      // 0xxx: SYSCLK not divided // AHB prescaler // 19.7

		RCC->CFGR &= ~RCC_CFGR_PPRE1;     // 0xx: HCLK not divided // APB low-speed prescaler (APB1)
		RCC->CFGR |= RCC_CFGR_PPRE1_2;    // 100: HCLK divided by 2 // APB low-speed prescaler (APB1)

		RCC->CR |= RCC_CR_PLLON;          // 19.5
		while ((RCC->CR & RCC_CR_PLLRDY) == 0) { ; }

		RCC->CFGR &= ~RCC_CFGR_SW;        // 00: HSI selected as system clock // 19.6
		RCC->CFGR |= RCC_CFGR_SW_1;       // 01: HSE selected as system clock; 10: PLL selected as system clock
		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1) { ; }

		RCC->CFGR &= ~RCC_CFGR_USBPRE;    // 0: PLL clock is divided by 1.5 // 19.10
}

void USB_Attach(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	//RCC->APB1ENR |= RCC_APB1RSTR_USBRST;  // ?? Is nessesary?
	//RCC->APB1ENR &= ~RCC_APB1RSTR_USBRST; // ?? Is nessesary?
}

void USB_Detach(void)
{
	RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
	// PA11, PA12
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRH &= ~(GPIO_CRH_CNF11 | GPIO_CRH_CNF12); // b00 - Output, Push-pull
	GPIOA->CRH |= GPIO_CRH_MODE11 | GPIO_CRH_MODE12;  // b11 Max. output speed 50 MHz
	GPIOA->BSRR = GPIO_BSRR_BR11 | GPIO_BSRR_BR12;
}

void USB_Reset(void)
{
	//USB->CNTR   = USB_CNTR_FRES;  // Forse Reset
	USB->ISTR  &= ~USB_ISTR_RESET;
	USB->CNTR   = 0;
	USB->ISTR   = 0;
	USB->BTABLE = 0;
	USB->DADDR = USB_DADDR_EF;    //Bit7 EF:Enable function
}


void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
#ifdef TEST_ON_TARGET

	USB_Detach();
	NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
	Clock_Configure();
	USB_Attach();
	USB_Reset();

#endif
}

void tearDown(void)
{
}

enum usb_stat_t {
	STAT_DISABLED = 0,
	STAT_STALL =    1,
	STAT_NAK =      2,
	STAT_VALID =    3,
};

enum usb_ep_type_kind_t {
	EP_TYPE_BULK =               0, // (0 << 1) | 0,
	EP_TYPE_BULK_DBL_BUF=        1, // (0 << 1) | 1,
	EP_TYPE_CONTROL =            2, // (1 << 1) | 0,
	EP_TYPE_CONTROL_STATUS_OUT = 3, // (1 << 1) | 1,
	EP_TYPE_ISO =                4, // (2 << 1) | 0,
	EP_TYPE_INTERRUPT =          6, // (3 << 1) | 0,
};



void test_init_endpoint(void)
{
	void ep0_configure(void)
	{
		uint16_t ep = USB->EP0R;
		ep = ep_write_ep_type_kind(ep, EP_TYPE_CONTROL);
		ep = ep_set_stat_rx(ep, STAT_VALID);
		USB->EP0R = ep;
	}

  const size_t btable_offset = 0;
  const size_t addr_tx = 0;
  const size_t count_tx = 0;
  const size_t addr_rx = 64;
  const size_t bl_size = 1, num_block = 2, count_rx = 0; // 32 bytes/block * 2 blocks

	volatile struct btable_ep_t *btable_ep = (uint8_t*)((struct btable_ep_t*)(PMA_ADDRESS)) + btable_offset*2;
	btable_ep[0] = (struct btable_ep_t){
		.tx_addr = addr_tx,
		.tx_count= count_tx,
		.rx_addr = addr_rx,
		.rx_count= {.BL_SIZE_TYPE = bl_size, .NUM_BLOCK = num_block, .COUNT_RX = count_rx,},
	};


	USB->BTABLE = btable_offset; // !! MUST BE ALLIGNED by 0b1000
	ep0_configure();


	while (1)	{
		uint16_t ep = USB->EP0R;
		if(ep_get_ctr_rx(ep) != 0){
			// USB_CTR_RX_new(ep) :
			//uint8_t rcvd_amount  = ep->btable_ep->rx_count.COUNT_RX & 0xFF;
			size_t rcvd_amount  = btable_ep[0].rx_count.COUNT_RX;
			for (size_t i = 0; i < rcvd_amount; ++i) {
				printf("0x%02X ", read_pma_u8(btable_ep[0].rx_addr + i));
			}
			fflush(stdout);
			__asm__("bkpt");
			//read_from_pma(ep->btable_ep->rx_addr, &input_buffer[0], rcvd_amount);
			//led_on();
		}

		if(USB->ISTR & USB_ISTR_RESET){
			//__asm__("bkpt");
			USB_Attach();
			USB_Reset();
			ep0_configure();
		}
	}
}
