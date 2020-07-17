#include "unity.h"
#include "binary_search.h"

void setUp(void)
{
}
 
void tearDown(void)
{
}
 
/*******************************************************************************
 *    TESTS
 ******************************************************************************/

char *ring_buffer;

static int read_rb(void *record, size_t index)
{
  *(char*)record = ring_buffer[index];
//  printf("read: index=%ld, record=%d\n", index, *(char*)record);
  if (ring_buffer[index] == (char)255) { // empty record
//    printf("empty record\n");
    return -1;
  }
  return 0;
}

static int compare_rb(void *record, void *value)
{
  int res = *(char*)record - *(char*)value;
//  printf("compare %d , %d, res = %d\n", *(char*)record, *(char*)value, res);
  return res;
}
#define LAST_INDEX(ARR) (sizeof(ARR) / sizeof(ARR[0]) - 1)

void test1(void)
{    
    char rec1, rec2, rec3;
    void* rec[3] = {&rec1, &rec2, &rec3};
	size_t last_index;

#define TESTING(ARR) do { ring_buffer = ARR; last_index = LAST_INDEX(ARR); } while(0);

	size_t res;
//               index:  0  1  2  3  4  
	char test_buf1[] = { 3, 4, 0, 1, 2, };	
	TESTING(test_buf1);
    res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 2, res);

//               index:  0  1  2  3  4  5
	char test_buf2[] = { 3, 4, 5, 0, 1, 2, };	
	TESTING(test_buf2);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 3, res);
	
	

}

/*----------------------------------------------------------------------------------*/

