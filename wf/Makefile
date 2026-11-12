CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -fsanitize=address,undefined

all: wf

wf: main.c wf.c wf.h stb_ds.h
	$(CC) $(CFLAGS) main.c wf.c -o wf

test: test_wf
	./test_wf

test_wf: test_wf.c wf.c wf.h stb_ds.h utest.h
	$(CC) $(CFLAGS) test_wf.c wf.c -o test_wf

clean:
	rm -f wf test_wf

.PHONY: all test clean
