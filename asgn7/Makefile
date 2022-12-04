CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra
LIBFLAGS = -lm

SOURCES  = $(wildcard *.c)

OBJECTS  = $(SOURCES:%.c=%.o)

.PHONY: all clean format spotless

all: encode decode

test: test.o code.o io.o node.o pq.o stack.o
	$(CC) -o $@ $^ $(LIBFLAGS)

encode: encode.o huffman.o code.o io.o node.o pq.o stack.o
	$(CC) -o $@ $^ $(LIBFLAGS)

decode: decode.o huffman.o code.o io.o node.o pq.o stack.o
	$(CC) -o $@ $^ $(LIBFLAGS)

clean:
	rm -f test encode decode $(OBJECTS)

spotless:
	rm -f test encode decode $(OBJECTS)

format:
	clang-format -i -style=file *.[ch]

%.o : %.c
	$(CC) $(CFLAGS) -c $<

