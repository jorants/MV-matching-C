.PHONY: default all clean doc lib solvers test

default: lib solvers test

all: default

lib:
	cd src && make

solvers:
	cd solvers && make

test: lib
	cd testsuit && make
#	cp bin/libmv.h testsuit

clean:
	rm -rf doc/*
	rm -rf bin/*
	cd src && make clean
	cd testsuit && make clean

doc:
	doxygen Doxyfile

