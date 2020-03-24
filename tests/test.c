#include "../include/test.h"

static void *
setup_test_data(const MunitParameter params[], void *user_data)
{
    int len = munit_rand_int_range(1, 20);
    int *fixed_arr = (int *)calloc(len, sizeof(int));
    for (int i = 0; i < len; i++)
    {
        fixed_arr[i] = i;
    }
    test_data *fixture = (test_data *)malloc(sizeof(test_data));
    fixture->len = len;
    fixture->arr = fixed_arr;
    fixture->arr1 = NULL;
    fixture->arr2 = NULL;
    return (void *)fixture;
}

static void teardown_test_data(void *fixture)
{
    test_data *cast_fixture = (test_data *)fixture;
    free(cast_fixture->arr);
    if (cast_fixture->arr1)
        array_free(cast_fixture->arr1);
    if (cast_fixture->arr2)
        array_free(cast_fixture->arr2);
    if (cast_fixture->arr3)
        array_free(cast_fixture->arr3);
    free(cast_fixture);
}

static MunitResult test_array_from(const MunitParameter params[], void *fixture)
{
    test_data *cast_fixture = (test_data *)fixture;
    int *fixed_arr = cast_fixture->arr;
    int len = cast_fixture->len;
    array *arr = array_from(fixed_arr, len);
    for (int i = 0; i < len; i++)
    {
        munit_assert_int(arr->_data[i], ==, fixed_arr[i]);
    }
    cast_fixture->arr1 = arr;
    return MUNIT_OK;
}

static MunitResult test_array_from_pop(const MunitParameter params[], void *fixture)
{
    test_data *cast_fixture = (test_data *)fixture;
    int *fixed_arr = cast_fixture->arr;
    int len = cast_fixture->len;
    array *arr = array_from(fixed_arr, len);
    for (int i = 0; i < len; i++)
    {
        munit_assert_int(array_pop(arr), ==, fixed_arr[len - i - 1]);
    }
    munit_assert_int(array_len(arr), ==, 0);
    // to free up during teardown
    cast_fixture->arr1 = arr;

    return MUNIT_OK;
}

static MunitResult test_array_push(const MunitParameter params[], void *fixture)
{
    array *arr = array_cstor();
    // to test array doubling and dynamic reallocation
    int len = munit_rand_int_range(1, 1e6);
    for (int i = 0; i < len; i++)
    {
        array_push(arr, i);
        munit_assert_int(arr->_data[i], ==, i);
    }
    for (int i = 0; i < len; i++)
    {
        munit_assert_int(arr->_data[i], ==, i);
    }
    // to free up during teardown
    test_data *cast_fixture = (test_data *)fixture;
    cast_fixture->arr1 = arr;

    return MUNIT_OK;
}

static MunitResult test_array_get(const MunitParameter params[], void *fixture)
{
    test_data *cast_fixture = (test_data *)fixture;
    int *fixed_arr = cast_fixture->arr;
    int len = cast_fixture->len;
    array *arr = array_from(fixed_arr, len);
    for (int i = 0; i < len; i++)
    {
        munit_assert_int(array_get(arr, i), ==, fixed_arr[i]);
    }
    // negative indices
    munit_assert_int(array_get(arr, -1), ==, array_get(arr, len - 1));
    if (len >= 2)
    {
        munit_assert_int(array_get(arr, -2), ==, array_get(arr, len - 2));
    }

    cast_fixture->arr1 = arr;

    return MUNIT_OK;
}

static MunitResult test_array_slice(const MunitParameter params[], void *fixture)
{
    test_data *cast_fixture = (test_data *)fixture;
    int *fixed_arr = cast_fixture->arr;
    int len = cast_fixture->len;

    array *arr = array_from(fixed_arr, len);
    int start = munit_rand_int_range(0, len - 1);
    int pos_end = munit_rand_int_range(start, len);
    int neg_end = munit_rand_int_range(start - len, -1);
    array *pos_slice = array_slice(arr, start, pos_end);
    array *neg_slice = array_slice(arr, start, neg_end);
    if (pos_end == start)
        munit_assert_int(pos_slice->len, ==, 0);
    else
    {
        munit_assert_memory_equal(sizeof(int) * (pos_end - start), pos_slice->_data, arr->_data + start);
    }
    if (neg_end == start - len)
        munit_assert_int(neg_slice->len, ==, 0);
    else
    {
        munit_assert_memory_equal(sizeof(int) * (len + neg_end - start), neg_slice->_data, arr->_data + start);
    }
    // to free up during teardown

    cast_fixture->arr1 = pos_slice;
    cast_fixture->arr2 = neg_slice;
    cast_fixture->arr3 = arr;

    return MUNIT_OK;
}

MunitTest tests[] = {
    {"/dynamic-array-from-static-array",
     test_array_from,
     setup_test_data,
     teardown_test_data,
     MUNIT_TEST_OPTION_SINGLE_ITERATION,
     NULL},
    {"/dynamic-array-pop-after-static-array-construction",
     test_array_from_pop,
     setup_test_data,
     teardown_test_data,
     MUNIT_TEST_OPTION_SINGLE_ITERATION,
     NULL},
    {"/dynamic-array-push",
     test_array_push,
     setup_test_data,
     teardown_test_data,
     MUNIT_TEST_OPTION_SINGLE_ITERATION,
     NULL},
    {"/dynamic-array-get",
     test_array_get,
     setup_test_data,
     teardown_test_data,
     MUNIT_TEST_OPTION_SINGLE_ITERATION,
     NULL},
    {"/dynamic-array-slice",
     test_array_slice,
     setup_test_data,
     teardown_test_data,
     MUNIT_TEST_OPTION_NONE,
     NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}};

static const MunitSuite suite = {
    "/dynamic-array-interface",
    tests,
    NULL,
    1000,
    MUNIT_SUITE_OPTION_NONE};

int main(int argc, const char *argv[])
{
    return munit_suite_main(&suite, NULL, 0, NULL);
}