all:
	gcc -o testgraphs testgraphs.c -L ../bin -lmv -ggdb
	gcc -o clean clean.c -L ../bin -lmv -ggdb
	gcc -o test test.c ../src/*.o -ggdb -pg

clean:
	rm -f mvwatching test testgraphs
