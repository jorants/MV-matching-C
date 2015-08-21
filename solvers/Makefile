all:
	gcc -O3 -L ../bin -o solver-libmv solver-libmv.c -lmv 
	g++ -O3 -L ../bin -o solver-boost solver-boost.cpp -lmv


clean:
	rm -f solver-libmv solver-boost
