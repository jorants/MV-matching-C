#include <stdlib.h>

#include "DDFSInfo.h"

DDFSInfo *
DDFSInfo_init (PointerList * petals)
{
  DDFSInfo *ddfsi = malloc (sizeof (DDFSInfo));

  ddfsi->bStack = PointerList_init ();
  ddfsi->rStack = PointerList_init ();

  ddfsi->bNumUnexausted = 0;
  ddfsi->rNumUnexausted = 0;

  ddfsi->petal = Petal_init (petals);

  return ddfsi;
}

void
DDFSInfo_delete (DDFSInfo * ddfsi)
{
  PointerList_delete (ddfsi->bStack);
  PointerList_delete (ddfsi->rStack);

  free (ddfsi);
}

void
DDFSInfo_push (DDFSInfo * ddfsi, DDFSColor color, NodeListIterator * itm)
{
  if (color == red)
    {
      PointerList_push (ddfsi->rStack, itm);
      if (itm->next)
	ddfsi->rNumUnexausted++;
    }
  else if (color == blue)
    {
      PointerList_push (ddfsi->bStack, itm);
      if (itm->next)
	ddfsi->bNumUnexausted++;
    }
}

uint
DDFSInfo_top (DDFSInfo * ddfsi, DDFSColor color)
{
  NodeListIterator *top;
  if (color == red)
    top = PointerList_top (ddfsi->rStack);
  else if (color == blue)
    top = PointerList_top (ddfsi->bStack);
  return top->value;
}
