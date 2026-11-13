# C Playground Notes

Learning pure C by building small programs. See `AGENTS.md` for the working rules.

## Projects

### `ds-demo/`: stb_ds.h and utest.h basics
- Dynamic arrays: `arrput`, `arrlen`, `arrpop`, `arrdel`, `arrdelswap`, `arrins`, `arrfree`
- String-key hash map: `sh_new_strdup`, `shput`, `shget`, `shgeti`, `shdel`, `shlen`, `shfree`
- Int-key hash map: `hmput`, `hmget`, `hmdel`, `hmlen`, `hmfree`
- Unit tests with `utest.h`: `UTEST(suite, name)`, `ASSERT_*`, `EXPECT_*`, `UTEST_MAIN()`
- Run: `cd ds-demo && make test`

### `wf/`: word-frequency counter (built test-first)

| Part | What | Status |
|---|---|---|
| 1 | Skeleton: `Makefile`, `wf.h`, `wf.c`, `main.c`, `test_wf.c` | Done |
| 2 | `wf_tokenize`: split text into lowercase words | Done |
| 3 | `wf_count`: word to count with a string hash map | Done |
| 4 | Sort by count descending, ties alphabetical | Next |
| 5 | CLI: file or stdin, `-n` option | Planned |
| 6 | Edge cases: empty file, missing file, long words | Planned |

Layout: logic in `wf.c`, CLI only in `main.c`, tests in `test_wf.c`.
Run: `cd wf && make test`

## Libraries (single-header, vendored)

| Library | Use | Notes |
|---|---|---|
| `stb_ds.h` | Arrays, hash maps | `STB_DS_IMPLEMENTATION` in exactly one `.c` file |
| `utest.h` | Unit tests | `UTEST_MAIN()` in exactly one `.c` file |

## Lessons learned

- **Never nest stb_ds macros.** `shput(m, k, shget(m, k) + 1)` modifies the map twice in one expression, which is undefined behavior. GCC warns with `-Wsequence-point`, and the sanitizer reports leaks. Read into a variable first, then write.
- **Start arrays and maps as `NULL`.** Call `sh_new_strdup(map)` before the first `shput`.
- **Don't keep a pointer to an array element across `arrput`.** The array may reallocate.
- **Check downloads.** `curl -fLO <url>` fails on a 404 instead of saving the error page. Watch for typos in the repo name (`stb`, not `std`).
- **Makefile recipes need tabs.** Check with `cat -A Makefile` (tabs show as `^I`).
- **List every header in the Makefile dependencies**, or edits will not trigger a rebuild.
- **Sanitizers catch real bugs.** Keep `-fsanitize=address,undefined` on.
- **Cast to `unsigned char`** before `isalpha` and `tolower`.
- **Don't assert on hash map iteration order.**
- **Never commit build artifacts** (`demo`, `wf`, `test_wf`). They are in `.gitignore`.
- **Test first.** Watch the test fail (red), implement, then watch it pass (green).

## Git history notes

- The `demo` binary was once committed. History was rewritten and force-pushed with `--force-with-lease`.
- Use `git commit --amend` only for unpushed commits.
- Push only when asked.

## Standard flags

```
-std=c11 -Wall -Wextra -g -fsanitize=address,undefined
```
(`ds-demo/Makefile` still lacks `-std=c11`.)

## Next steps

1. `wf` Part 4: sort counts with `qsort` and a comparator.
2. `wf` Part 5: CLI with `argc`/`argv`, file or stdin, and `-n`.
3. Add `-std=c11` to `ds-demo/Makefile`.
4. Next utility ideas: `mygrep`, a `sort`/`uniq -c` clone, `hexdump`, a key-value store.
