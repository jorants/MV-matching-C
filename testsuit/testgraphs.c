#include <stdio.h>
#include <stdlib.h>



#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"


main(int argc,char** argv)
{
  uint i, j;


  MVInfo *mvi = MVInfo_init_file(argv[1]);

  mvi->pathc = 1;
  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  EdgeListIterator * current = matching->first;
  int siz = 0;
  while(current) {
    current = current-> next;
    siz++;
  }
  printf("size: %i\ncount: %i\n",siz,mvi->matched_num);
  // deallocate
  EdgeList_delete(matching);
  Graph *g = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(g);
  return 0;
}
