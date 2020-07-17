#include <stddef.h>
#include "binary_search.h"

// TODO: index_right is included! so right = index_right + 1;
ptrdiff_t leftmost_binary_search(size_t index_left, size_t index_right, void *value, comparer_t *compare, element_reader_t *read_element, void *element_ptr)
{
	ptrdiff_t index_diff;
	while(index_right > index_left)
	{
		size_t index_middle = (index_left + index_right) / 2; //  m = l + diff/2 --> m = (l + r)/2 = (l + (l + diff))/2 = (2*l + diff)/2 = l + diff/2
		if ((*read_element)(element_ptr, index_middle) != 0)
		{     // Error during reading means "record is empty"
			index_right = index_middle;                   //  r = m;
			continue;
		}
		if ((*compare)(element_ptr, value) < 0)             //  arr[m] < val ?
		{
			index_left = index_middle + 1;                //  l = m + 1;
		}
		else
		{
			index_right = index_middle;                   //  r = m;
		}
	}

	/*if ((*compare)(element_ptr, value) != 0)
	{
		return -1;
	}*/

	return index_left;                                            // return l
}

/*---------------------------------------------------------------------------------------------------*/
// TODO: index_right is included! so right = index_right + 1;
ptrdiff_t find_minimum(size_t index_left, size_t index_right, comparer_t *compare, element_reader_t *read_element, void* element[3])
{ 

  if ((*read_element)(element[0], index_left) < 0) { // { X, X , X, X, X, X } where X is an empty record
    return -1;
  }

  if ((*read_element)(element[2], index_right) < 0) { // { 0, 1 , 2, X, X, X } where X is an empty record
    return index_left;
  }

  while((*compare)(element[0], element[2]) > 0) {               // arr[l] > arr[r]
    size_t index_middle = (index_left + index_right) / 2;       //
    int res = (*read_element)(element[1], index_middle);
    if (res < 0) { // Error during reading
      return -1;
    }
    if ((*compare)(element[1], element[2]) > 0) {               //  arr[m] > arr[r] ?  i.e 6>2 in {3,4,5,6,0,1,2}
      index_left = index_middle + 1;                            //  l = m + 1;
      if ((*read_element)(element[0], index_left) < 0) {        //  file error. we've read "X" from an array like that --> { 3, 4 , 5, X, 0, 1, 2 } where X is an empty record
        return -1;
      }
    } else {
      index_right = index_middle;                               //  r = m;
      // TODO: optimize here. We don't need to read arr[r] because
      // r=m now and we've already read arr[m] into element2
      if ((*read_element)(element[2], index_right) < 0) {       //  file error. we've read "X" from an array like that --> { 3, 4 , 5, X, 0, 1, 2 } where X is an empty record
        return -1;
      }
    }
  }

  return index_left;                                             // return l
}

/*---------------------------------------------------------------------------------------------------*/
// TODO: index_right is included! so right = index_right + 1;
ptrdiff_t find_ring_buffer_element(size_t index_left, size_t index_right, void* value, comparer_t *compare, element_reader_t *read_element, void* element[3])
{
	size_t left;
	size_t right;
	size_t n;
	// Find pivot
	size_t res = find_minimum(index_left, index_right, compare, read_element, element);
	
	if (res < 0) { // empty file
		return -1;
	}
	
	// left sub-array
	if (res > 0) {
		left = index_left;
		right = res - 1;
		n = leftmost_binary_search( left, right, value, compare, read_element, element[0]);
		if ((n >= index_left) && (n <= index_right)) {
			(*read_element)(element[1], n);		
			if ((*compare)(element[1], value) == 0) {
				return n;
			}			
		}
	}	
	// right sub-array
	left = res;
	right = index_right;
	n = leftmost_binary_search( left, right, value, compare, read_element, element[0]);	
	if ((n >= index_left) && (n <= index_right)) {
		(*read_element)(element[1], n);		
		if ((*compare)(element[1], value) == 0) {
			return n;
		}			
	}	
	
	return -1;
}