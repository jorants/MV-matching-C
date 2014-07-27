#include "graphtypes.h"

main(argc,argv)
int argc;
char *argv[];

{	int *Mate;
	Graph graph;
	int i,size;

	graph = ReadGraph(&size,argv[1]);

	Mate = Match(graph);

	for (i=1; i<=size; i++)
		printf("%d %d\n",i,Mate[i]);
}

