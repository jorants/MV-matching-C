#include <stdlib.h>

#include "PointerList.h"

PointerListIterator *
PointerListIterator_init (PTR_LIST_TYPE val)
{
  PointerListIterator *elm = malloc (sizeof (PointerListIterator));
  elm->next = 0;
  elm->value = val;
  return elm;
}

PointerList *
PointerList_init ()
{
  PointerList *lst = malloc (sizeof (PointerList));
  lst->first = 0;
  lst->last = 0;
  return lst;
}

void
PointerList_delete (PointerList * lst)
{
  if (PointerList_is_empty (lst))
    {
      free (lst);
    }
  else
    {
      PointerListIterator *itr = lst->first;
      while (itr)
	{
	  PointerListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      free (lst);
    }
}

bool
PointerList_is_empty (PointerList * lst)
{
  assert (lst != 0);
  return lst->first == 0 || lst->last == 0;
}

bool
PointerList_is_empty_or_singleton (PointerList * lst)
{
  assert (lst != 0);
  return lst->first == lst->last;
}

void
PointerList_add (PointerList * lst, PTR_LIST_TYPE val)
{
  if (PointerList_is_empty (lst))
    {
      lst->first = lst->last = PointerListIterator_init (val);
    }
  else
    {
      PointerListIterator *newelm = PointerListIterator_init (val);
      lst->last->next = newelm;
      lst->last = newelm;
    }

}

void
PointerList_push (PointerList * lst, PTR_LIST_TYPE val)
{
  if (PointerList_is_empty (lst))
    {
      lst->first = lst->last = PointerListIterator_init (val);
    }
  else
    {
      PointerListIterator *newelm = PointerListIterator_init (val);
      newelm->next = lst->first;
      lst->first = newelm;
    }
}

PTR_LIST_TYPE
PointerList_pop (PointerList * lst)
{
  assert (!PointerList_is_empty (lst));

  PointerListIterator *top = lst->first;
  lst->first = top->next;
  if (top->next == 0)
    lst->last = 0;
  PTR_LIST_TYPE val = top->value;
  free (top);
  return val;
}

PTR_LIST_TYPE
PointerList_top (PointerList * lst)
{
  assert (!PointerList_is_empty (lst));

  return lst->first->value;
}

PTR_LIST_TYPE
PointerList_nth (PointerList * lst, uint n)
{
  PointerListIterator *itr = lst->first;
  while (n != 0)
    {
      assert (itr != 0);
      itr = itr->next;
      n--;
    }
  assert (itr != 0);
  return itr->value;
}

void
PointerList_set_top (PointerList * lst, PTR_LIST_TYPE newtop)
{
  assert (!PointerList_is_empty (lst));
  lst->first->value = newtop;
}

void
PointerList_erase (PointerList * lst)
{
  if (!PointerList_is_empty (lst))
    {
      PointerListIterator *itr = lst->first;
      while (itr)
	{
	  PointerListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      lst->first = lst->last = 0;
    }
}

void
PointerList_append (PointerList * to, PointerList * from)
{
  if (!PointerList_is_empty (from))
    {
      PointerListIterator *itr = from->first;
      while (itr)
	{
	  PointerList_add (to, itr->value);
	  itr = itr->next;
	}
    }
}

void
PointerList_append_reverse_aux (PointerList * to, PointerListIterator * itr)
{
  if (itr->next)
    PointerList_append_reverse_aux (to, itr->next);
  PointerList_add (to, itr->value);
}

void
PointerList_append_reverse (PointerList * to, PointerList * from)
{
  if (!PointerList_is_empty (from))
    {
      PointerListIterator *itr = from->first;
      PointerList_append_reverse_aux (to, itr);
    }
}

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void
PointerList_join (PointerList * to, PointerList * from)
{
  if (PointerList_is_empty (to))
    {
      to->first = from->first;
      to->last = from->last;
    }
  else
    {
      to->last->next = from->first;
      to->last = from->last;
    }
  from->first = 0;
  from->last = 0;
  free (from);
}

bool
PointerList_find (PointerList * lst, PTR_LIST_TYPE elm)
{
  PointerListIterator *itr = lst->first;
  while (itr)
    {
      if (PTR_LIST_TYPE_EQUALS (itr->value, elm))
	return true;
      itr = itr->next;
    }
  return false;
}
