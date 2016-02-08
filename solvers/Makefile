all: solver-libmv solver-libmv-print solver-boost solver-lemon

FLAGS=-g -O0 -L ../bin

solver-libmv: solver-libmv.c ../bin/libmv.a
	gcc $(FLAGS) -o solver-libmv solver-libmv.c -lmv 
	
solver-libmv-print: solver-libmv-print.c ../bin/libmv.a
	gcc $(FLAGS) -o solver-libmv-print solver-libmv-print.c -lmv 

solver-boost: solver-boost.cpp ../bin/libmv.a
	g++ $(FLAGS) -o solver-boost solver-boost.cpp -lmv

solver-lemon: solver-lemon.cpp ../bin/libmv.a
	g++ $(FLAGS) -o solver-lemon solver-lemon.cpp -lmv


clean:
	rm -f solver-libmv solver-libmv-print solver-boost solver-lemon
