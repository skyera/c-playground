#ifndef WF_H
#define WF_H
#include <stdio.h>

/* Split text into lowercase ASCII words (runs of letters).
 * Returns a stb_ds array of malloc'd strings; NULL if there are none.
 * Free with wf_free_words(). */
char **wf_tokenize(const char *text);
void wf_free_words(char **words);

/* One word and how many times it occurred. Used as a stb_ds string-map entry. */
typedef struct {
    char *key;
    int value;
} WfCount;

/* Count occurrences of each word. Returns a stb_ds string map (word -> count).
 * Keys are copied, so `words` may be freed afterwards. Free with wf_free_counts(). */
WfCount *wf_count(char **words);
void wf_free_counts(WfCount *counts);

/* Return the entries of `counts` sorted by count (highest first), then by word (A-Z).
 * The result is a stb_ds array of WfCount whose keys are BORROWED from `counts`:
 * free it with arrfree() only, and do not use it after wf_free_counts(counts). */
WfCount *wf_sorted(WfCount *counts);

/* Read all of `fp` into a NUL-terminated malloc'd string.
 * Returns NULL on read or allocation failure. The caller frees it with free(). */
char *wf_read_all(FILE *fp);

#endif
