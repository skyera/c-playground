CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address,undefined

test: demo
	./demo

demo: main.c stb_ds.h utest.h
	$(CC) $(CFLAGS) main.c -o demo

clean:
	rm -f demo
