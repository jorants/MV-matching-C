#include <stdio.h>
#include <stdlib.h>



#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"


main(int argc,char** argv)
{

  //Load a graph file in DIRAC format
  MVInfo *mvi = MVInfo_init_file(argv[1]);
  // Does the actual matching:
  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);

  //Get head of edgelist
  EdgeListIterator * current = matching->first;
  int size = 0;
  while(current) {
    printf("matched: %i  %i\n",current->value.v1,current->value.v2);
    current = current->next;
    size++;
  }
  printf("matching number: %i\n",size);

  // deallocate
  EdgeList_delete(matching);
  Graph *g = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(g);

  return 0;
}
