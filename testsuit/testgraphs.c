#include <stdio.h>
#include <stdlib.h>


#include "libmv.h"


main()
{
  uint i, j;


  MVInfo *mvi = MVInfo_init_file("g_error.txt");


  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  EdgeListIterator * current = matching->first;
  int siz = 0;
  while(current) {
    current = current-> next;
    siz++;
  }
  printf("size: %i\n",siz);
  // deallocate
  EdgeList_delete(matching);
  Graph *g = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(g);
}
