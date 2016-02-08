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
  
  if (!file_exists(filename)){
      printf("No such file %s\n", filename);
      exit(-1);
  }
  
  MVInfo *mvi = MVInfo_init_file(filename);
  mvi->pathc = 1;

  clock_t start_time, finish_time;
  start_time = clock();

  MVInfo_set_output(mvi, "mv_progress");
  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  
  finish_time = clock();
  
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
  printf("The algorithm took %.3f seconds.\n", (double)(finish_time - start_time) / CLOCKS_PER_SEC);


  return 0; // everything went OK
}
