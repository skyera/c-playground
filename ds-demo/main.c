#include <stdio.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

typedef struct {
    char *key;
    int value;
} Entry;

int main(void) {
    int *arr = NULL;
    for (int i = 1; i <= 5; i++)
        arrput(arr, i * 10);

    printf("array length: %td\n", arrlen(arr));
    for (int i = 0; i < arrlen(arr); i++)
        printf("arr[%d] = %d\n", i, arr[i]);

    /* --- string-key hash map --- */
    Entry *map = NULL;
    sh_new_strdup(map);
    shput(map, "apple", 3);
    shput(map, "banana", 7);
    shput(map, "apple", 4);

    printf("apple = %d\n", shget(map, "apple"));
    printf("banana = %d\n", shget(map, "banana"));
    printf("map size: %td\n", shlen(map));

    for (int i = 0; i < shlen(map); i++)
        printf(" %s -> %d\n", map[i].key, map[i].value);

    /* --- cleanup --- */
    arrfree(arr);
    shfree(map);
    return 0;
}
