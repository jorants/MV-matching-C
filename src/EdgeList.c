#include <stdlib.h>

#include "EdgeList.h"

EdgeListIterator *
EdgeListIterator_init (EDGE_LIST_TYPE val)
{
  EdgeListIterator *elm = malloc (sizeof (EdgeListIterator));
  elm->next = 0;
  elm->value = val;
  return elm;
}

EdgeList *
EdgeList_init ()
{
  EdgeList *lst = malloc (sizeof (EdgeList));
  lst->first = 0;
  lst->last = 0;
  return lst;
}

void
EdgeList_delete (EdgeList * lst)
{
  if (EdgeList_is_empty (lst))
    {
      free (lst);
    }
  else
    {
      EdgeListIterator *itr = lst->first;
      while (itr)
	{
	  EdgeListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      free (lst);
    }
}

bool
EdgeList_is_empty (EdgeList * lst)
{
  assert (lst != 0);
  return lst->first == 0 || lst->last == 0;
}

bool
EdgeList_is_empty_or_singleton (EdgeList * lst)
{
  assert (lst != 0);
  return lst->first == lst->last;
}

void
EdgeList_add (EdgeList * lst, EDGE_LIST_TYPE val)
{
  if (EdgeList_is_empty (lst))
    {
      lst->first = lst->last = EdgeListIterator_init (val);
    }
  else
    {
      EdgeListIterator *newelm = EdgeListIterator_init (val);
      lst->last->next = newelm;
      lst->last = newelm;
    }

}

void
EdgeList_push (EdgeList * lst, EDGE_LIST_TYPE val)
{
  if (EdgeList_is_empty (lst))
    {
      lst->first = lst->last = EdgeListIterator_init (val);
    }
  else
    {
      EdgeListIterator *newelm = EdgeListIterator_init (val);
      newelm->next = lst->first;
      lst->first = newelm;
    }
}

EDGE_LIST_TYPE
EdgeList_pop (EdgeList * lst)
{
  assert (!EdgeList_is_empty (lst));

  EdgeListIterator *top = lst->first;
  lst->first = top->next;
  if (top->next == 0)
    lst->last = 0;
  EDGE_LIST_TYPE val = top->value;
  free (top);
  return val;
}

EDGE_LIST_TYPE
EdgeList_top (EdgeList * lst)
{
  assert (!EdgeList_is_empty (lst));

  return lst->first->value;
}

EDGE_LIST_TYPE
EdgeList_nth (EdgeList * lst, uint n)
{
  EdgeListIterator *itr = lst->first;
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
EdgeList_set_top (EdgeList * lst, EDGE_LIST_TYPE newtop)
{
  assert (!EdgeList_is_empty (lst));
  lst->first->value = newtop;
}

void
EdgeList_erase (EdgeList * lst)
{
  if (!EdgeList_is_empty (lst))
    {
      EdgeListIterator *itr = lst->first;
      while (itr)
	{
	  EdgeListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      lst->first = lst->last = 0;
    }
}

void
EdgeList_append (EdgeList * to, EdgeList * from)
{
  if (!EdgeList_is_empty (from))
    {
      EdgeListIterator *itr = from->first;
      while (itr)
	{
	  EdgeList_add (to, itr->value);
	  itr = itr->next;
	}
    }
}

void
EdgeList_append_reverse_aux (EdgeList * to, EdgeListIterator * itr)
{
  if (itr->next)
    EdgeList_append_reverse_aux (to, itr->next);
  EdgeList_add (to, itr->value);
}

void
EdgeList_append_reverse (EdgeList * to, EdgeList * from)
{
  if (!EdgeList_is_empty (from))
    {
      EdgeListIterator *itr = from->first;
      EdgeList_append_reverse_aux (to, itr);
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
EdgeList_join (EdgeList * to, EdgeList * from)
{
  if (EdgeList_is_empty (to))
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
EdgeList_find (EdgeList * lst, EDGE_LIST_TYPE elm)
{
  EdgeListIterator *itr = lst->first;
  while (itr)
    {
      if (EDGE_LIST_TYPE_EQUALS (itr->value, elm))
	return true;
      itr = itr->next;
    }
  return false;
}
