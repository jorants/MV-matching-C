/**
  @file
*/

#ifndef PTRLIST_H
#define PTRLIST_H

#include "basictypes.h"

#define PTR_LIST_TYPE void*
#define PTR_LIST_TYPE_EQUALS(x, y) (x == y)

typedef struct PointerListIterator
{
  PTR_LIST_TYPE value;
  struct PointerListIterator *next;
} PointerListIterator;


/**
 typical use
PointerList *lst = PointerList_init();
..
PointerList_add(lst, element)
...
PointerListIterator *itr = lst->first;
while (itr != 0) {
	uint elm = itr->value;
	
	... do stuff with elm...

	itr = itr->next;
}
*/
typedef struct
{
  PointerListIterator *first, *last;
} PointerList;

PointerList *PointerList_init ();
void PointerList_delete (PointerList * lst);

bool PointerList_is_empty (PointerList * lst);
bool PointerList_is_empty_or_singleton (PointerList * lst);

void PointerList_add (PointerList * lst, PTR_LIST_TYPE elm);
void PointerList_push (PointerList * lst, PTR_LIST_TYPE elm);

PTR_LIST_TYPE PointerList_pop (PointerList * lst);	// removes first element and returns it
PTR_LIST_TYPE PointerList_top (PointerList * lst);	// returns first element
PTR_LIST_TYPE PointerList_nth (PointerList * lst, uint n);	// returns nth element (first = 0th)


void PointerList_set_top (PointerList * lst, PTR_LIST_TYPE newtop);

void PointerList_erase (PointerList * lst);	// empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void PointerList_append (PointerList * to, PointerList * from);
void PointerList_append_reverse (PointerList * to, PointerList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void PointerList_join (PointerList * to, PointerList * from);


bool PointerList_find (PointerList * lst, PTR_LIST_TYPE elm);

#endif // PTRLIST_H
