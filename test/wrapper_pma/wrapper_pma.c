#include "wrapper_pma.h"

#ifdef TEST
  #ifdef TEST_ON_TARGET
    #include "stm32f1xx.h"
    #define PMA_ADDRESS ((uintptr_t)0x40006000)
  #else
    extern uint8_t pool[];
    #define PMA_ADDRESS (&pool[0])
  #endif
#endif

#include "pma.c"
