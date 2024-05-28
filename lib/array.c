#include <malloc.h>
#include <assert.h>

#include "array.h"

array_t* ctor_array(FILE *fp)
{
    assert(fp != NULL);

    ssize_t sz_file = calculate_size_file(fp);
    assert(sz_file >= 0);

    char *array = (char *)calloc(sz_file + 1, sizeof(char));

    size_t count_read_char = fread(array, sizeof(char), sz_file, fp);
    assert(count_read_char == sz_file);
    array[sz_file] = '\0';
    
    array_t *struct_arr    = (array_t *)malloc(sizeof(array_t));
    assert(struct_arr != NULL);

    struct_arr->array      = array;
    struct_arr->size_array = sz_file;

    return struct_arr;
}

int dtor_array(array_t *arr)
{
    free(arr->array);
    free(arr);

    return 0;
}

ssize_t calculate_size_file(FILE *fp)
{
    assert(fp != NULL);

    ssize_t current_pos = ftell(fp);
    assert(current_pos != -1L);
 
    fseek(fp, 0L, SEEK_END);

    ssize_t size_file = ftell(fp);
    assert(size_file != -1L);

    fseek(fp, current_pos, SEEK_SET);

    return size_file;
}