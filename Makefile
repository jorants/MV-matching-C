.PHONY: default all clean doc lib solvers test

default: lib solvers test options

all: default

lib:
	cd src && make

solvers:
	cd solvers && make

test: lib
	cd testsuit && make
#	cp bin/libmv.h testsuit

options:
	cd options && make

clean:
	rm -rf doc/*
	rm -rf bin/*
	cd src && make clean
	cd testsuit && make clean
	cd options && make clean

doc:
	doxygen Doxyfile


