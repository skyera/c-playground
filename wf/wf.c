#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "wf.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int wf_version(void) {
    return 1;
}

void wf_free_words(char **words) {
    for (ptrdiff_t i = 0; i < arrlen(words); i++) {
        free(words[i]);
    }
    arrfree(words);
}

char **wf_tokenize(const char *text) {
    char **words = NULL;
    if (text == NULL) {
        return NULL;
    }

    const char *p = text;
    while (*p != '\0') {
        while (*p != '\0' && !isalpha((unsigned char)*p)) {
            p++;
        }
        const char *start = p;
        while (isalpha((unsigned char)*p)) {
            p++;
        }
        size_t len = (size_t)(p - start);
        if (len == 0) {
            break;
        }

        char *word = malloc(len + 1);
        if (word == NULL) {
            wf_free_words(words);
            return NULL;
        }
        for (size_t i = 0; i < len; i++) {
            word[i] = (char)tolower((unsigned char)start[i]);
        }
        word[len] = '\0';
        arrput(words, word);
    }
    return words;
}

WfCount *wf_count(char **words) {
    WfCount *counts = NULL;
    sh_new_strdup(counts);
    for (ptrdiff_t i = 0; i < arrlen(words); i++) {
        int n = shget(counts, words[i]);
        shput(counts, words[i], n + 1);
    }
    return counts;
}

void wf_free_counts(WfCount *counts) {
    shfree(counts);
}

static int compare_counts(const void *a, const void *b) {
    const WfCount *x = a;
    const WfCount *y = b;
    if (x->value != y->value) {
        return (x->value < y->value) ? 1 : -1;
    }
    return strcmp(x->key, y->key);
}

WfCount *wf_sorted(WfCount *counts) {
    WfCount *sorted = NULL;
    for (ptrdiff_t i = 0; i < shlen(counts); i++) {
        arrput(sorted, counts[i]);
    }
    if (arrlen(sorted) > 1) {
        qsort(sorted, (size_t)arrlen(sorted), sizeof *sorted, compare_counts);
    }
    return sorted;
}
