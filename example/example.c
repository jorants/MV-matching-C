#include <stdio.h>
#include <stdlib.h>


#include "libmv.h"


int main(int argc,char** argv)
{

  //Load a graph file in SIMPLE format
  Graph *g = Graph_init_file_simple(argv[1]);

  // Does the actual matching:
  EdgeList *matching = MV_MaximumCardinalityMatching(g);

  //Get head of edgelist
  EdgeListIterator * current = matching->first;
  int size = 0;
  while(current) {
    printf("matched: %i  %i\n",current->value.v1,current->value.v2);
    current = current-> next;
    size++;
  }
  printf("matching number: %i\n",size);

  // deallocate
  EdgeList_delete(matching);
  Graph_delete(g);

  return 0;
}
