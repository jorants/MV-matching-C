#include <stdlib.h>

#include "Petal.h"



// creates a new petal, indexes it in the given pointer list
Petal *
Petal_init (PointerList * ptrs)
{
  Petal *ptl = malloc (sizeof (Petal));

  ptl->support = NodeList_init ();

  ptl->rednode = INFINITY;
  ptl->bluenode = INFINITY;
  ptl->base = INFINITY;
  ptl->link = INFINITY;
  ptl->linkholder = NULL;

  PointerList_add (ptrs, ptl);

  return ptl;
}


// Deletes all petals indexed in the pointer list
// does NOT delete the pointerlist itself
void
Petal_deleteAll (PointerList * ptrs)
{
  PointerListIterator *itr = ptrs->first;

  while (itr != 0)
    {
      Petal *ptl = (Petal *) itr->value;
      NodeList_delete (ptl->support);
      free (ptl);
      itr = itr->next;
    }

  PointerList_erase (ptrs);
}
