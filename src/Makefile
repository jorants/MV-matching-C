CC=gcc
CFLAGS= -c
LIBS = -lm
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))


%.o: %.c
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

.PHONY: all
all: default

.PHONY: default
default: $(OBJECTS)
	rm -f libmv.a
	ar -cvq ../bin/libmv.a $^

.PHONY: clean
clean:
	rm -f *.o *.a
