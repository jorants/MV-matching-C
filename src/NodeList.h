/**
   @file
*/
#ifndef NODELIST_H
#define NODELIST_H

#include "basictypes.h"

#define LIST_TYPE uint
#define LIST_TYPE_EQUALS(x, y) (x == y)

typedef struct NodeListIterator
{
  LIST_TYPE value;
  struct NodeListIterator *next;
} NodeListIterator;

/**
   typical use
   NodeList *lst = NodeList_init();
   ..
   NodeList_add(lst, element)
   ...
   NodeListIterator *itr = lst->first;
   while (itr != 0) {
   uint elm = itr->value;

   ... do stuff with elm...

   itr = itr->next;
   }
*/
typedef struct
{
  NodeListIterator *first, *last;
} NodeList;

NodeList *NodeList_init ();
void NodeList_delete (NodeList * lst);

bool NodeList_is_empty (NodeList * lst);
bool NodeList_is_empty_or_singleton (NodeList * lst);

void NodeList_add (NodeList * lst, LIST_TYPE elm);
void NodeList_push (NodeList * lst, LIST_TYPE elm);

LIST_TYPE NodeList_pop (NodeList * lst);	// removes first element and returns it
LIST_TYPE NodeList_top (NodeList * lst);	// returns first element
LIST_TYPE NodeList_nth (NodeList * lst, uint n);	// returns nth element (first = 0th)


void NodeList_set_top (NodeList * lst, LIST_TYPE newtop);

void NodeList_erase (NodeList * lst);	// empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void NodeList_append (NodeList * to, NodeList * from);
void NodeList_append_reverse (NodeList * to, NodeList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void NodeList_join (NodeList * to, NodeList * from);


bool NodeList_find (NodeList * lst, LIST_TYPE elm);


#endif // NODELIST_H
