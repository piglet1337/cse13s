CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -gdwarf-4
LIBFLAGS = -lm

SOURCES  = $(wildcard *.c)

OBJECTS  = $(SOURCES:%.c=%.o)

.PHONY: all clean format spotless

all: encode decode

test: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

encode: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

decode: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

clean:
	rm -f test encode decode $(OBJECTS)

spotless:
	rm -f test encode decode $(OBJECTS)

format:
	clang-format -i -style=file *.[ch]

%.o : %.c
	$(CC) $(CFLAGS) -c $<

