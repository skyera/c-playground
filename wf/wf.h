#ifndef WF_H
#define WF_H

int wf_version(void);

/* Split text into lowercase ASCII words (runs of letters).
 * Returns a stb_ds array of malloc'd strings; NULL if there are none.
 * Free with wf_free_words(). */
char **wf_tokenize(const char *text);
void wf_free_words(char **words);
#endif
