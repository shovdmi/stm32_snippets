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

const char *ring_buffer;

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
#define TESTING(ARR) do { ring_buffer = ARR; last_index = LAST_INDEX(ARR); } while(0);

void test1(void)
{    
    char rec1, rec2, rec3;
    void* rec[3] = {&rec1, &rec2, &rec3};
	size_t last_index;
	size_t res;

//               index:  0  1  2  3  4  
	const char test_buf1[] = { 3, 4, 0, 1, 2, };	
	TESTING(test_buf1);
    res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 2, res);

//               index:  0  1  2  3  4  5
	const char test_buf2[] = { 3, 4, 5, 0, 1, 2, };	
	TESTING(test_buf2);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 3, res);
	
	/* Partially filled file, where 0xFF -- empty record */
	const char test_buf3[] = { 3, 4, 5, 6, 0xFF, 0xFF};
	TESTING(test_buf3);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 0, res);

	/*  "Empty" file, where 0xFF -- empty record */
	const char test_buf4[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };	
	TESTING(test_buf4);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( -1, res);
}

/*----------------------------------------------------------------------------------*/

void test2(void)
{   
	char x;
    char rec1, rec2, rec3;
    void* rec[3] = {&rec1, &rec2, &rec3};
	size_t last_index;

#define TESTING(ARR) do { ring_buffer = ARR; last_index = LAST_INDEX(ARR); } while(0);

	size_t res;
//                     index:  0  1  2  3  4  
	const char test_buf1[] = { 3, 4, 0, 1, 2, };	
	TESTING(test_buf1);
    res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 2, res);
	
	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(1, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(3, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(4, res);
	
	x = -1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	

//                     index:  0  1  2  3  4  5
	const char test_buf2[] = { 3, 4, 5, 0, 1, 2, };	
	TESTING(test_buf2);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 3, res);

	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(1, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(2, res);
	
	x = 0;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(3, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(4, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(5, res);
	
	x = -1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);


	
	/* Partially filled file, where 0xFF -- empty record */
//                     index:  0  1  2  3  4     5	
	const char test_buf3[] = { 3, 4, 5, 6, 0xFF, 0xFF};
	TESTING(test_buf3);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 0, res);

	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(1, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(2, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 6;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(3, res);
	
	x = 0xFF;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);



	/*  "Empty" file, where 0xFF -- empty record */
	const char test_buf4[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };	
	TESTING(test_buf4);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( -1, res);

	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 0xFF;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	
	
	/* One element file */
	const char test_buf5[] = { 4, };
	TESTING(test_buf5);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( 0, res);

	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(0, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 0xFF;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);	
	
	
	
	/* One element "empty" file */
	const char test_buf6[] = { 0xFF, };
	TESTING(test_buf6);
	res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    TEST_ASSERT_EQUAL_INT( -1, res);

	x = 4;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 5;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 1;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 2;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);
	
	x = 0xFF;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);

	x = 15;
	res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
	TEST_ASSERT_EQUAL_INT(-1, res);	


	
	/* Zero elements file */
	const char test_buf7 = { 0, };
	//TESTING(test_buf7);
	//last_index = 0; means arr[0..0] i.e. lenth == 1
	//last_index = -1; error because (size_t)last_index >= 0
	//res = find_ring_buffer_element(0, last_index, &x, compare_rb, read_rb, rec);
}