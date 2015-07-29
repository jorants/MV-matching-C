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



  FILE *fp;
  if (argc <2){
    fp = stdin;
  }else{
    fp = fopen (argv[1], "r");
  }


  //this is just initfile, but doesnt use the filename:
  uint size, numedge;
    
  fscanf (fp, "p edge %d %d\n", &size, &numedge);

  Graph *g = Graph_init (size);
  MVInfo *mvi = MVInfo_init (g);
  char line[30];
  uint i, j, tmp;
  while ((fscanf (fp, "e %i %i %i\n", &i, &j, &tmp)) != EOF)
    {
      Graph_add_edge (g, i - 1, j - 1);
    }
  mvi->stage = -1;
  mvi->output = false;
  MVInfo_next_stage (mvi);
  mvi->pathc = 1;
  if(argc>=2)
    fclose (fp);


  // times
  struct timeval  tv1, tv2;

  //start timing
  gettimeofday(&tv1, NULL);


  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);

  gettimeofday(&tv2, NULL);
  double msec = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
  printf("%f\n",msec);

  
  EdgeListIterator * current = matching->first;
  int siz = 0;
  while(current) {
    current = current-> next;
    siz++;
  }
  // deallocate
  EdgeList_delete(matching);
  Graph *gres = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(gres);
  
  //printf("%d\n", siz);


  return 0; // everything went OK
}
