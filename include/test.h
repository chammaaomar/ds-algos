#include "../munit/munit.h"
#include "../include/array.h"

typedef struct
{
    int *arr;
    int len;
    // arrays I will dynamically assign and will have to free
    array *arr1;
    array *arr2;
    array *arr3;
} test_data;