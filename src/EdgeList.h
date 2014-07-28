/**
 @file
*/
#ifndef EDGELIST_H
#define EDGELIST_H

#include "basictypes.h"

#define EDGE_LIST_TYPE Edge
#define EDGE_LIST_TYPE_EQUALS(x, y) (((x.v1 == y.v1) && (x.v2 == y.v2))||((x.v1 == y.v2) && (x.v2 == y.v1)))

typedef struct EdgeListIterator
{
  EDGE_LIST_TYPE value;
  struct EdgeListIterator *next;
} EdgeListIterator;


/**
 typical use
EdgeList *lst = EdgeList_init();
..
EdgeList_add(lst, element)
...
EdgeListIterator *itr = lst->first;
while (itr != 0) {
	uint elm = itr->value;
	
	... do stuff with elm...

	itr = itr->next;
}
*/
typedef struct
{
  EdgeListIterator *first, *last;
} EdgeList;

EdgeList *EdgeList_init ();
void EdgeList_delete (EdgeList * lst);

bool EdgeList_is_empty (EdgeList * lst);
bool EdgeList_is_empty_or_singleton (EdgeList * lst);

void EdgeList_add (EdgeList * lst, EDGE_LIST_TYPE elm);
void EdgeList_push (EdgeList * lst, EDGE_LIST_TYPE elm);

EDGE_LIST_TYPE EdgeList_pop (EdgeList * lst);	// removes first element and returns it
EDGE_LIST_TYPE EdgeList_top (EdgeList * lst);	// returns first element
EDGE_LIST_TYPE EdgeList_nth (EdgeList * lst, uint n);	// returns nth element (first = 0th)


void EdgeList_set_top (EdgeList * lst, EDGE_LIST_TYPE newtop);

void EdgeList_erase (EdgeList * lst);	// empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void EdgeList_append (EdgeList * to, EdgeList * from);
void EdgeList_append_reverse (EdgeList * to, EdgeList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void EdgeList_join (EdgeList * to, EdgeList * from);


bool EdgeList_find (EdgeList * lst, EDGE_LIST_TYPE elm);


#endif // EDGELIST_H
