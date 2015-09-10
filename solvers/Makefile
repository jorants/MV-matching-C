all: solver-libmv solver-boost solver-lemon

solver-libmv: solver-libmv.c ../bin/libmv.a
	gcc -O3 -L ../bin -o solver-libmv solver-libmv.c -lmv 

solver-boost: solver-boost.cpp ../bin/libmv.a
	g++ -O3 -L ../bin -o solver-boost solver-boost.cpp -lmv

solver-lemon: solver-lemon.cpp ../bin/libmv.a
	g++ -O3 -L ../bin -o solver-lemon solver-lemon.cpp -lmv


clean:
	rm -f solver-libmv solver-boost
