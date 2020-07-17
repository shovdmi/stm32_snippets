#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "binary_search.h"

//             0  1  2  3  4  5  6  7  8
char arr[] = { 1, 2, 3, 4, 4, 4, 5, 9, 10};

int cmp(char *x, char *y)
{
	printf(" (%d vs %d) ", *x, *y);
	return (*x - *y);
}

int reader(char *x, size_t idx)
{
	*x = arr[idx];
	printf("arr[%ld]==%d\n", idx, arr[idx]);
	return 0;
}

int main(int argc, char** argv)
{
	char c;
	char x = 1;
	printf("\nleftmost %d at %ld\n", x, leftmost_binary_search(0, sizeof(arr), &x, (comparer_t*)&cmp, (element_reader_t*)&reader, &c));
	return 0;
}
