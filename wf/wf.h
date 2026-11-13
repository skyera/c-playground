#ifndef WF_H
#define WF_H

int wf_version(void);

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

#endif
