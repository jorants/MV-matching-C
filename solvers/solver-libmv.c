#include <dirent.h>
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <time.h>
#include <string.h>

#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"


int main (int argc,char** argv) // entry point of the program
{
  if (argc != 2) {
    printf("Usage: %s <name of file in DIMACS format>\n", argv[0]);
    exit(-1);
  }

  char *filename = argv[1];
  
  MVInfo *mvi = MVInfo_init_file(filename);
  mvi->pathc = 1;

  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  EdgeListIterator * current = matching->first;
  int siz = 0;
  while(current) {
    current = current-> next;
    siz++;
  }
  // deallocate
  EdgeList_delete(matching);
  Graph *g = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(g);
  
  printf("%d\n", siz);


  return 0; // everything went OK
}
