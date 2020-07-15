typedef int (element_reader_t)(void* element, size_t index); // returns -1 on error
typedef int (comparer_t)(void* value1,void* value2);

extern ptrdiff_t leftmost_binary_search(size_t index_left, size_t index_right, void *value, comparer_t *compare, element_reader_t *read_element, void* element_ptr);

