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

