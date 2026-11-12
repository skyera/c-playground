CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address,undefined

demo: main.c stb_ds.h
	$(CC) $(CFLAGS) main.c -o demo

clean:
	rm -r demo
