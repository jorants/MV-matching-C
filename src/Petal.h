/**
 @file
*/
#ifndef PETAL_H
#define PETAL_H

#define BLOSOM_DELETED ((unsigned int) -2)

#include "basictypes.h"
#include "NodeList.h"
#include "PointerList.h"


typedef struct
{
  uint bluenode, rednode, base;
  uint link;
  uint* linkholder;
  NodeList *support;
} Petal;


/// creates a new petal, indexes it in the given pointer list
Petal *Petal_init (PointerList * ptrs);
/// Deletes all petals indexed in the pointer list
/// does NOT delete the pointerlist itself,
/// and the pointers to the deleted petals become invalid
void Petal_deleteAll (PointerList * ptrs);

#endif // PETAL_H
