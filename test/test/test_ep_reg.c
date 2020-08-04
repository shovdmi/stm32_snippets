#include "unity.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



void setUp(void)
{
#ifdef RDIMON
	initialise_monitor_handles();
#endif
}

void tearDown(void)
{
}


uint8_t rw[8] = {
	//    vrm
	0, // 000 = 0
	0, // 001 = 1
	1, // 010 = 2
	0, // 011 = 3
	0, // 100 = 4
	1, // 101 = 5
	1, // 110 = 6
	1, // 111 = 7
};

uint8_t w0[8] = {
	//    vrm
	0, // 000 = 0
	0, // 001 = 1
	1, // 010 = 2
	0, // 011 = 3
	0, // 100 = 4
	0, // 101 = 5
	1, // 110 = 6
	1, // 111 = 7
};


uint8_t t[8] = {
	//    vrm
	0, // 000 = 0
	0, // 001 = 1
	1, // 010 = 2
	1, // 011 = 3
	0, // 100 = 4
	1, // 101 = 5
	1, // 110 = 6
	0, // 111 = 7
};

void test_tables(void)
{
	for (size_t v = 0; v < 2; v++)
	{
		for (size_t r = 0; r < 2; r++)
		{
			for (size_t m = 0; m < 2; m++)
			{
				uint8_t index = (v<<2) | (r<<1) | (m);
				
				uint8_t rw1 = rw[index];
				uint8_t w01 = w0[index];
				uint8_t rt1 = t[index];
				
				uint8_t rw2 = r;
				uint8_t w02 = r;
				uint8_t rt2 = r;
				if (m == 1) {
					 //read-write
					rw2 = v;
					
					 // write-0
					if (v == 0) {
						w02 = 0;
					}
					
					// toggle;
					rt2 = r ^ v;
				}
				TEST_ASSERT_EQUAL_UINT8(rw1, rw2);
				TEST_ASSERT_EQUAL_UINT8(w01, w02);
				TEST_ASSERT_EQUAL_UINT8(rt1, rt2);				
			}
		}
	}
}
