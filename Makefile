.PHONY: default all clean doc lib test

default: lib doc

all: default

lib:
	cd src && make


test: lib
	cd testsuit && make

clean:
	rm -rf doc/*
	rm -rf bin/*
	cd src && make clean

doc:
	doxygen Doxyfile
