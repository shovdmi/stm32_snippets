typedef int (element_reader_t)(void* element, size_t index); // returns -1 on error
typedef int (comparer_t)(void* value1,void* value2);

extern ptrdiff_t leftmost_binary_search(size_t index_left, size_t index_right, void *value, comparer_t *compare, element_reader_t *read_element, void* element_ptr);

extern ptrdiff_t find_minimum(size_t index_left, size_t index_right, comparer_t *compare, element_reader_t *read_element, void* element[3]);

extern ptrdiff_t find_ring_buffer_element(size_t index_left, size_t index_right, void* value, comparer_t *compare, element_reader_t *read_element, void* element[3]);