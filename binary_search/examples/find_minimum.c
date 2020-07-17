#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "../binary_search.h"

//                  0  1  2  3  4  5
char ring_buf[] = { 3, 4, 5, 0, 1, 2, };

static void print_array(void)
{
	printf("{ ");
	for (size_t i = 0; i < sizeof(ring_buf); i++){
		printf("%d, ", ring_buf[i]);
	}
	printf("}\n");
}

static int read_rb(void *record, size_t index)
{
  if (index >= sizeof(ring_buf)) {
	  return -1;
  }
  
  *(char*)record = ring_buf[index];
//  printf("read: index=%ld, record=%d\n", index, *(char*)record);
  if (ring_buf[index] == (char)255) { // empty record
    printf("empty record\n");
    return -1;
  }
  return 0;
}

static int compare_rb(void *record, void *value)
{
  int res = *(char*)record - *(char*)value;
  printf("compare %d , %d, res = %d\n", *(char*)record, *(char*)value, res);
  return res;
}



int main(int argc, char **argv)
{
  uint32_t value = 6;
  {
    printf("Rotary array binary search\n");	
	print_array();
	
    size_t last_index = sizeof(ring_buf)/sizeof(ring_buf[0]) - 1;
    char rec1, rec2, rec3;
    void* rec[3] = {&rec1, &rec2, &rec3};

    ptrdiff_t res = find_minimum(0, last_index, compare_rb, read_rb, rec);
    if (res < 0)  {
      printf("minimum not found (probably array is empty)\n" );
    }
    else {
      char rec;
      printf("minimum is ring_buf[%ld] = %d\n\n\n", res, ring_buf[res]);
    }
    return 0;
  }
}