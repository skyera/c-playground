#include "stb_ds.h"
#include <stdlib.h>
#include <string.h>
#include "utest.h"
#include "wf.h"

UTEST(tokenize, basic) {
    char **w = wf_tokenize("Hello, World!");
    ASSERT_EQ(2, (int)arrlen(w));
    ASSERT_STREQ("hello", w[0]);
    ASSERT_STREQ("world", w[1]);
    wf_free_words(w);
}

UTEST(tokenize, empty_and_null) {
    char **a = wf_tokenize("");
    char **b = wf_tokenize(NULL);
    ASSERT_EQ(0, (int)arrlen(a));
    ASSERT_EQ(0, (int)arrlen(b));
    wf_free_words(a);
    wf_free_words(b);
}

UTEST(tokenize, only_separators) {
    char **w = wf_tokenize("  ... ,, 123 !! ");
    ASSERT_EQ(0, (int)arrlen(w));
    wf_free_words(w);
}

UTEST(tokenize, lowercase) {
    char **w = wf_tokenize("MiXeD CASE");
    ASSERT_EQ(2, (int)arrlen(w));
    ASSERT_STREQ("mixed", w[0]);
    ASSERT_STREQ("case", w[1]);
    wf_free_words(w);
}

UTEST(tokenize, repeats_and_newlines) {
    char **w = wf_tokenize("a b\na\tb a");
    ASSERT_EQ(5, (int)arrlen(w));
    ASSERT_STREQ("a", w[0]);
    ASSERT_STREQ("a", w[4]);
    wf_free_words(w);
}

UTEST(count, repeats) {
    char **w = wf_tokenize("a b a c b a");
    WfCount *c = wf_count(w);
    ASSERT_EQ(3, (int)shlen(c));
    ASSERT_EQ(3, shget(c, "a"));
    ASSERT_EQ(2, shget(c, "b"));
    ASSERT_EQ(1, shget(c, "c"));
    wf_free_words(w);
    wf_free_counts(c);
}

UTEST(count, empty) {
    WfCount *c = wf_count(NULL);
    ASSERT_EQ(0, (int)shlen(c));
    wf_free_counts(c);
}

UTEST(count, missing_word) {
    char **w = wf_tokenize("apple");
    WfCount *c = wf_count(w);
    ASSERT_TRUE(shgeti(c, "banana") < 0);
    wf_free_words(w);
    wf_free_counts(c);
}

UTEST(count, case_insensitive) {
    char **w = wf_tokenize("The the THE");
    WfCount *c = wf_count(w);
    ASSERT_EQ(1, (int)shlen(c));
    ASSERT_EQ(3, shget(c, "the"));
    wf_free_words(w);
    wf_free_counts(c);
}

UTEST(count, survives_freeing_words) {
    char **w = wf_tokenize("keep keep");
    WfCount *c = wf_count(w);
    wf_free_words(w);
    ASSERT_EQ(2, shget(c, "keep"));
    wf_free_counts(c);
}

UTEST(sorted, by_count_desc) {
    char **w = wf_tokenize("a b a c b a");
    WfCount *c = wf_count(w);
    WfCount *s = wf_sorted(c);
    ASSERT_EQ(3, (int)arrlen(s));
    ASSERT_STREQ("a", s[0].key);
    ASSERT_EQ(3, s[0].value);
    ASSERT_STREQ("b", s[1].key);
    ASSERT_STREQ("c", s[2].key);
    arrfree(s);
    wf_free_counts(c);
    wf_free_words(w);
}

UTEST(sorted, ties_alphabetical) {
    char **w = wf_tokenize("pear apple banana");
    WfCount *c = wf_count(w);
    WfCount *s = wf_sorted(c);
    ASSERT_EQ(3, (int)arrlen(s));
    ASSERT_STREQ("apple", s[0].key);
    ASSERT_STREQ("banana", s[1].key);
    ASSERT_STREQ("pear", s[2].key);
    arrfree(s);
    wf_free_counts(c);
    wf_free_words(w);
}

UTEST(sorted, count_then_alpha) {
    char **w = wf_tokenize("b b a a c");
    WfCount *c = wf_count(w);
    WfCount *s = wf_sorted(c);
    ASSERT_EQ(3, (int)arrlen(s));
    ASSERT_STREQ("a", s[0].key);
    ASSERT_STREQ("b", s[1].key);
    ASSERT_STREQ("c", s[2].key);
    arrfree(s);
    wf_free_counts(c);
    wf_free_words(w);
}

UTEST(sorted, empty) {
    WfCount *c = wf_count(NULL);
    WfCount *s = wf_sorted(c);
    ASSERT_EQ(0, (int)arrlen(s));
    arrfree(s);
    wf_free_counts(c);
}

static FILE *tmp_with(const char *text, size_t len) {
    FILE *fp = tmpfile();
    if (fp != NULL) {
        fwrite(text, 1, len, fp);
        rewind(fp);
    }
    return fp;
}

UTEST(read_all, small) {
    FILE *fp = tmp_with("hello", 5);
    ASSERT_TRUE(fp != NULL);
    char *s = wf_read_all(fp);
    ASSERT_TRUE(s != NULL);
    ASSERT_STREQ("hello", s);
    free(s);
    fclose(fp);
}

UTEST(read_all, empty) {
    FILE *fp = tmp_with("", 0);
    ASSERT_TRUE(fp != NULL);
    char *s = wf_read_all(fp);
    ASSERT_TRUE(s != NULL);
    ASSERT_EQ(0, (int)strlen(s));
    free(s);
    fclose(fp);
}

UTEST(read_all, boundary_and_large) {
    size_t sizes[] = {4094, 4095, 4096, 10000};
    for (size_t k = 0; k < sizeof sizes / sizeof sizes[0]; k++) {
        char *text = malloc(sizes[k]);
        ASSERT_TRUE(text != NULL);
        memset(text, 'a', sizes[k]);
        FILE *fp = tmp_with(text, sizes[k]);
        ASSERT_TRUE(fp != NULL);
        char *s = wf_read_all(fp);
        ASSERT_TRUE(s != NULL);
        EXPECT_EQ(sizes[k], strlen(s));
        free(s);
        fclose(fp);
        free(text);
    }
}

UTEST_MAIN()
