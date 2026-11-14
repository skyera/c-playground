#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_ds.h"
#include "wf.h"

enum {
    EXIT_RUNTIME = 1,
    EXIT_USAGE = 2
};

static void usage(FILE *out) {
    fprintf(out, "usage: wf [-n N] [FILE]\n");
}

static int parse_positive(const char *s, long *out) {
    char *end = NULL;
    errno = 0;
    long value = strtol(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || value < 1) {
        return -1;
    }
    *out = value;
    return 0;
}

int main(int argc, char **argv) {
    long limit = 0;
    const char *path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            usage(stdout);
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[i], "-n") == 0) {
            if (i + 1 >= argc || parse_positive(argv[i + 1], &limit) != 0) {
                fprintf(stderr, "wf: -n needs a positive integer\n");
                usage(stderr);
                return EXIT_USAGE;
            }
            i++;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "wf: unknown option: %s\n", argv[i]);
            usage(stderr);
            return EXIT_USAGE;
        } else if (path == NULL) {
            path = argv[i];
        } else {
            fprintf(stderr, "wf: only one file is supported\n");
            usage(stderr);
            return EXIT_USAGE;
        }
    }

    FILE *fp = stdin;
    if (path != NULL) {
        fp = fopen(path, "r");
        if (fp == NULL) {
            fprintf(stderr, "wf: %s: %s\n", path, strerror(errno));
            return EXIT_RUNTIME;
        }
    }

    char *text = wf_read_all(fp);
    if (path != NULL) {
        fclose(fp);
    }

    if (text == NULL) {
        fprintf(stderr, "wf: failed to read input\n");
        return EXIT_RUNTIME;
    }

    char **words = wf_tokenize(text);
    WfCount *counts = wf_count(words);
    WfCount *sorted = wf_sorted(counts);

    ptrdiff_t shown = arrlen(sorted);
    if (limit > 0 && limit < shown) {
        shown = limit;
    }
    for (ptrdiff_t i = 0; i < shown; i++) {
        printf("%7d  %s\n", sorted[i].value, sorted[i].key);
    }

    arrfree(sorted);
    wf_free_counts(counts);
    wf_free_words(words);
    free(text);
    return EXIT_SUCCESS;
}
