/**
 @file
*/
#ifndef DDFS_INFO_H
#define DDFS_INFO_H

#include "NodeList.h"
#include "PointerList.h"
#include "Petal.h"

// DDFS Colors
typedef enum
{ none = 0, red, blue } DDFSColor;

typedef struct
{
  PointerList *bStack, *rStack;
  // blue and red unexausted nodes in stack
  uint bNumUnexausted, rNumUnexausted;
  Petal *petal;

} DDFSInfo;

DDFSInfo *DDFSInfo_init (PointerList * petals);

void DDFSInfo_delete (DDFSInfo *);

void DDFSInfo_push (DDFSInfo *, DDFSColor, NodeListIterator *);
uint DDFSInfo_top (DDFSInfo *, DDFSColor);

#endif // DDFS_INFO_H
