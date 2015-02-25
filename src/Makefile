CC=gcc
CFLAGS= -c -g -fPIC
LIBS = -lm
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))


%.o: %.c
	$(CC) $(LIBS) $(CFLAGS) -o $@ $<

.PHONY: all
all: default

.PHONY: default
default: $(OBJECTS)
	rm -f ../bin/libmv.a
	ar -cvq ../bin/libmv.a $^
	cp libmv.h ../bin/libmv.h
#	cpp -C MV.h | grep " uint" -A 10000  | grep -v "^#" > ../bin/libmv.h
.PHONY: clean
clean:
	rm -f *.o *.a
