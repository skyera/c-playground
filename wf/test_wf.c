#include "stb_ds.h"
#include "utest.h"
#include "wf.h"

UTEST(wf, version) {
    ASSERT_EQ(1, wf_version());
}

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

UTEST_MAIN()
