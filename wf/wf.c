#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#include "wf.h"

#include <ctype.h>
#include <stdlib.h>

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
