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
  if (argc <3){
    printf("Not engough arguments\n Usage: %s [V] [p]\n",argv[0]);
  }
  
  int V = atoi(argv[1]);
  double p = atof(argv[2]);
    
  Graph *g = Graph_init(V);
  MVInfo *mvi = MVInfo_init (g);

  srandom(7); //temp, change this to the time!
  
  int a,b;
  for(a=0;a<V-1;a++)
  {
    for(b=a+1;b<V;b++)
      {
	if(((float)random())/(RAND_MAX) <= p)
	  Graph_add_edge (g, a, b);
      }
  }
  mvi->stage = -1;
  mvi->output = false;
  MVInfo_next_stage (mvi);
  mvi->pathc = 1;
  


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
  


  return 0; // everything went OK
}
