#include <stddef.h>
#include "binary_search.h"

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

size_t find_minimum(size_t index_left, size_t index_right, comparer_t *compare, element_reader_t *read_element, void* element[3])
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