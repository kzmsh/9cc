CFLAGS=-std=c11 -g -static

9cc: main.c
	$(CC) -o $@ $? $(LDFLAGS)

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
