typedef struct
{
    int *_data;
    int _sz; // upper bound before re-allocating
    int len;
} array;

array *array_cstor();

array *array_from(int *source, unsigned int len); // copy constructor

int array_free(array *arr);

int array_push(array *arr, int num);

int array_pop(array *arr);

unsigned int array_len(array *arr);

int array_get(array *arr, int index);

array *array_slice(array *arr, int start, int end);
