# AGENTS.md: Pure C Programming Guide

This repo is a C playground. The user is **learning C by doing it themselves**.
These rules add to the parent `/home/zliu/AGENTS.md` (research → plan → approval → implement → verify; one commit per issue; push only when told).

## Teaching mode (default)

- **Guide, don't take over.** Give numbered steps with the exact commands and code. The user types and runs them.
- **One step at a time, with a checkpoint.** After each step, state the expected output so the user can compare. Wait for their result before moving on.
- **The user types everything by default.** When the user says "I will type", "guide me", or "explain", do NOT create, edit, delete, or commit files. Show the full file content or a before/after diff in chat and let the user apply it.
- **Only edit files or run commands yourself when the user says** "do it", "apply", "update ... directly", or similar. That permission covers the named file or task only, not later requests.
- **"check" means inspect and verify, not modify.** Read files, run builds and tests, and report. Never fix things during a check.
- **Explain the why in one or two lines**, not a lecture. Point out common pitfalls (see below).
- **Verify before you hand over.** Test snippets in the scratchpad first so the expected output is real. If something was not run, say so.
- **Review the user's code honestly.** Say what is correct, then list real issues in priority order.

## Language rules: pure C

- Language: **C11** (`-std=c11`). No C++ features, no C++ compilers, no `.cpp` files.
- Compiler: `gcc` (clang also fine). Keep code portable: standard library first, POSIX only when needed and said so.
- Prefer simple, readable C: small functions, `static` for file-local helpers, `const` where it fits, `size_t` for sizes.
- Always check return values of `malloc`, `fopen`, and similar calls.
- Every allocation has exactly one owner and one `free`. Say who owns what.
- No VLAs and no undefined behavior (signed overflow, out-of-bounds, uninitialized reads, use-after-free).

## C coding style

Match the existing code in `ds-demo/main.c`. Follow these when writing or reviewing code:

- **Formatting:** 4 spaces, no tabs in `.c`/`.h` files (tabs are only for Makefile recipes). Lines ≤ 100 columns. One statement per line.
- **Braces:** opening brace on the same line (K&R), including functions. Always use braces for `if`/`for`/`while` bodies that span more than one line.
- **Naming:** `snake_case` for functions and variables, `UPPER_CASE` for macros and constants, `PascalCase` only for `typedef`'d struct names, as in `Entry`. Prefix public functions with the module name (`list_push`, not `push`).
- **Declarations:** declare variables where first used, one per line, and initialize them. `static` for file-local functions and globals. Avoid mutable globals.
- **`const` and types:** `const` on pointer parameters that are not modified. Use `size_t` for sizes and counts, `<stdint.h>` fixed-width types for sized data, and `bool` from `<stdbool.h>` for flags.
- **Pointers:** write `char *name`, with the `*` next to the name. Initialize pointers to `NULL` and set them to `NULL` after `free`.
- **Functions:** small, doing one thing. Return an error code or `NULL` on failure and document who frees what. Prefer early returns over deep nesting.
- **Headers:** every `.h` has an include guard (`#ifndef MODULE_H` / `#define MODULE_H` / `#endif`). Include only what you use, and never define non-`static` variables or functions in headers (single-header libraries excepted).
- **Macros:** avoid them when a `static inline` function works. Parenthesize macro arguments and the whole body.
- **Comments:** explain why, not what. No commented-out code.
- **Switch:** always include a `default` case, and mark intentional fall-through with a comment.
- **Magic numbers:** name them with `enum` or `#define`.

## Build

Default flags for every program and test:

```
-std=c11 -Wall -Wextra -g -fsanitize=address,undefined
```

- The `ds-demo/Makefile` currently omits `-std=c11`. Add it to `CFLAGS` when convenient. New Makefiles should include it from the start.
- Use a `Makefile` with a `test` target and a `clean` target (`rm -f`).
- **Recipe lines must start with a tab.** Check with `cat -A Makefile` (tabs show as `^I`).
- List every header a target depends on, so edits trigger rebuilds.
- Warnings are bugs. Fix them. Do not silence them globally. A warning from inside a vendored library header may be noted and ignored.

## Libraries in use

| Purpose | Library | Notes |
|---|---|---|
| Dynamic arrays, hash maps | `stb_ds.h` | Define `STB_DS_IMPLEMENTATION` in exactly one `.c` file. Start arrays and maps as `NULL`. Call `sh_new_strdup(map)` before the first `shput`. |
| Unit tests | `utest.h` | `UTEST(suite, name) { ASSERT_EQ(...); }` and one `UTEST_MAIN()`. Include at least one assertion per test. |

- Single-header libraries are vendored into the repo and committed.
- Download with `curl -fLO <url>`. **`-f` makes a 404 fail** instead of saving an error page. Capital letter **O**. Check the repo name and path carefully, then verify with `ls -l` and `head -2`.

## Testing and verification

- Every behavior change gets a test. Run `make test` before every commit.
- Test boundary cases: empty input, one element, `NULL`, overwrite, missing key.
- Do not assert on hash map iteration order.
- Sanitizer output must be clean: no leaks, no errors.

## Git

- Never commit build artifacts (`demo`, `test_*`, `*.o`). Check `git status --short` before committing.
- Commit message: short imperative subject. Proofread it. Use `git commit --amend` for local typos.
- Commit locally when told "commit". Push only when told "push". Rewriting pushed history needs explicit approval, and prefer `--force-with-lease`.

## Common pitfalls to call out

- Forgetting `= NULL` on a stb_ds array or map.
- Defining `STB_DS_IMPLEMENTATION` in more than one file.
- Keeping a pointer to an array element across `arrput` (it may reallocate).
- Spaces instead of tabs in a Makefile.
- Mixing a program's `main()` with `UTEST_MAIN()`. Keep tests and programs in separate files once the project grows.
