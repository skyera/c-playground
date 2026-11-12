#include <stdio.h>

#include "utest.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
    char *key;
    int value;
} Entry;

UTEST(array, put_and_len) {
    int *arr = NULL;
    for (int i = 1; i <= 5; i++)
        arrput(arr, i * 10);
    ASSERT_EQ(5, (int)arrlen(arr));
    ASSERT_EQ(10, arr[0]);
    ASSERT_EQ(50, arr[4]);
    arrfree(arr);
}

UTEST(map, put_get_overwrite) {
    Entry *map = NULL;

    sh_new_strdup(map);
    shput(map, "apple", 3);
    shput(map, "banana", 7);
    shput(map, "apple", 4);
    ASSERT_EQ(2, (int)shlen(map));
    ASSERT_EQ(4, shget(map, "apple"));
    ASSERT_EQ(7, shget(map, "banana"));
    shfree(map);
}

UTEST(array, pop) {
    int *arr = NULL;
    arrput(arr, 7);
    arrput(arr, 8);
    ASSERT_EQ(8, arrpop(arr));
    ASSERT_EQ(1, (int)arrlen(arr));
    ASSERT_EQ(7, arr[0]);
    arrfree(arr);
}

UTEST_MAIN()
