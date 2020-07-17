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

//           index:  0  1  2  3  4  5  6  7  8
char test1_arr[] = { 1, 2, 3, 4, 4, 4, 5, 9, 10};
static int test1_cmp(char *x, char *y)
{
//	printf(" (%d vs %d) ", *x, *y);
	return (*x - *y);
}

static int test1_reader(char *x, size_t idx)
{
	*x = test1_arr[idx];
//	printf("arr[%ld]==%d\n", idx, test1_arr[idx]);
	return 0;
}
 
void test1(void)
{
	char c;
	size_t left =0;
	size_t right = sizeof(test1_arr);
	char x = 1;
	//printf("\nleftmost %d at %ld\n", x, leftmost_binary_search(0, sizeof(test1_arr), &x, (comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));
	
	x = 1;
    TEST_ASSERT_EQUAL_INT( 0, leftmost_binary_search( left, right, &x, \
			(comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));

	x = 10;
    TEST_ASSERT_EQUAL_INT(8, leftmost_binary_search( left, right, &x, \
			(comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));
	
	/* Testing for absent values */
	x = 0;
    TEST_ASSERT_EQUAL_INT(0, leftmost_binary_search( left, right, &x, \
			(comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));

	x = 11;
    TEST_ASSERT_EQUAL_INT(9, leftmost_binary_search( left, right, &x, \
			(comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));
			
	x = 7;
    TEST_ASSERT_EQUAL_INT(7, leftmost_binary_search( left, right, &x, \
			(comparer_t*)&test1_cmp, (element_reader_t*)&test1_reader, &c));

}

/*----------------------------------------------------------------------------------*/

