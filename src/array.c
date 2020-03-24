#include <stdlib.h>
#include "../include/array.h"

array *array_cstor()
{
    array *arr = (array *)malloc(sizeof(array));
    arr->_sz = 16;
    arr->len = 0;
    arr->_data = (int *)calloc(arr->_sz, sizeof(int));

    return arr;
}

array *array_from(int *source, unsigned int len)
{
    array *copy_arr = (array *)malloc(sizeof(array *));
    copy_arr->len = len;
    copy_arr->_sz = 2 * len;
    copy_arr->_data = (int *)malloc(sizeof(int) * copy_arr->_sz);
    for (int i = 0; i < copy_arr->len; i++)
    {
        (copy_arr->_data)[i] = source[i];
    }

    return copy_arr;
}

int array_push(array *arr, int num)
{
    if (arr->len >= arr->_sz)
    {
        arr->_sz *= 2;
        arr->_data = (int *)realloc(arr->_data, sizeof(int) * arr->_sz);
    }

    (arr->_data)[(arr->len)++] = num;
    return 0;
}

int array_pop(array *arr)
{
    if (!arr->len)
        return 0;
    int popped = (arr->_data)[arr->len - 1];
    if (arr->len-- <= arr->_sz / 4 + 1)
    {
        arr->_sz /= 2;
        arr->_data = (int *)realloc(arr->_data, sizeof(int) * arr->_sz);
    }
    return popped;
}

int array_get(array *arr, int index)
{
    if (!arr->len)
        return 0;
    index = index < 0 ? arr->len + index : index;
    return (arr->_data)[index > arr->len - 1 ? arr->len - 1 : index];
}

array *array_slice(array *arr, int start, int end)
{
    start = start < 0 ? arr->len + start : start;
    end = end < 0 ? arr->len + end : end;
    if (end <= start)
    {
        // return an empty array
        array *slice = array_cstor();
        return slice;
    }
    array *slice = array_from(arr->_data + start, end - start);
    return slice;
}

unsigned int array_len(array *arr)
{
    return arr->len;
}

int array_free(array *arr)
{
    free(arr->_data);
    free(arr);
    return 0;
}