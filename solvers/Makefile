all:
	gcc -O3 -L ../bin -o solver-libmv solver-libmv.c -lmv
	gcc -O3 -L ../bin -o solver-libmvrand libmvrandom.c -lmv 
	g++ -O3 -o solver-boost solver-boost.cpp
	g++ -O3 -o boostrandom boostrandom.cpp


clean:
	rm -f solver-libmv solver-boost boostrandom libmvrandom
