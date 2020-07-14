typedef int (element_reader_t)(void* element, size_t index); // returns -1 on error
typedef int (comparer_tape_t)(void* value1,void* value2);

ptrdiff_t leftmost_binary_search(size_t index_left, size_t index_right, void *value, comparer_t *compare, element_reader_t *read_element, void* element_ptr)
{	
	ptrdiff_t index_diff;
	while(index_right > index_left) {
		size_t index_middle = (index_left + index_right) / 2; //  m = l + diff/2 --> m = (l + r)/2 = (l + (l + diff))/2 = (2*l + diff)/2 = l + diff/2
		if ((*read_element)(element_ptr, index_middle)) {     // Error during reading
			return -1;
		}
		if ((*compare)(element_ptr, value) < 0) {             //  arr[m] < val ?
			index_left = index_middle + 1;                //  l = m + 1;
		} else {
			index_right = index_middle;                   //  r = m;
		}
	}
	
	if ((*read_element)(element_ptr, index_left)) // Error during reading			
		return -1;
	}
	if ((*compare)(element_ptr, value) != 0) {		
		return -1;
	}
 
	return index_left;                                            // return l
}


// TODO: usage : if ((index_left >=0) && (index_left < array_size)) { ...
