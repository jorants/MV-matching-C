#include <stdlib.h>

#include "NodeList.h"

NodeListIterator *
NodeListIterator_init (LIST_TYPE val)
{
  NodeListIterator *elm = malloc (sizeof (NodeListIterator));
  elm->next = 0;
  elm->value = val;
  return elm;
}

NodeList *
NodeList_init ()
{
  NodeList *lst = malloc (sizeof (NodeList));
  lst->first = 0;
  lst->last = 0;
  return lst;
}

void
NodeList_delete (NodeList * lst)
{
  if (NodeList_is_empty (lst))
    {
      free (lst);
    }
  else
    {
      NodeListIterator *itr = lst->first;
      while (itr)
	{
	  NodeListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      free (lst);
    }
}

bool
NodeList_is_empty (NodeList * lst)
{
  assert (lst != 0);
  return lst->first == 0 || lst->last == 0;
}

bool
NodeList_is_empty_or_singleton (NodeList * lst)
{
  assert (lst != 0);
  return lst->first == lst->last;
}

void
NodeList_add (NodeList * lst, LIST_TYPE val)
{
  if (NodeList_is_empty (lst))
    {
      lst->first = lst->last = NodeListIterator_init (val);
    }
  else
    {
      NodeListIterator *newelm = NodeListIterator_init (val);
      lst->last->next = newelm;
      lst->last = newelm;
    }

}

void
NodeList_push (NodeList * lst, LIST_TYPE val)
{
  if (NodeList_is_empty (lst))
    {
      lst->first = lst->last = NodeListIterator_init (val);
    }
  else
    {
      NodeListIterator *newelm = NodeListIterator_init (val);
      newelm->next = lst->first;
      lst->first = newelm;
    }
}

LIST_TYPE
NodeList_pop (NodeList * lst)
{
  assert (!NodeList_is_empty (lst));

  NodeListIterator *top = lst->first;
  lst->first = top->next;
  if (top->next == 0)
    lst->last = 0;
  LIST_TYPE val = top->value;
  free (top);
  return val;
}

LIST_TYPE
NodeList_top (NodeList * lst)
{
  assert (!NodeList_is_empty (lst));

  return lst->first->value;
}

LIST_TYPE
NodeList_nth (NodeList * lst, uint n)
{
  NodeListIterator *itr = lst->first;
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
NodeList_set_top (NodeList * lst, LIST_TYPE newtop)
{
  assert (!NodeList_is_empty (lst));
  lst->first->value = newtop;
}

void
NodeList_erase (NodeList * lst)
{
  if (!NodeList_is_empty (lst))
    {
      NodeListIterator *itr = lst->first;
      while (itr)
	{
	  NodeListIterator *dlc = itr;
	  itr = itr->next;
	  free (dlc);
	}
      lst->first = lst->last = 0;
    }
}

void
NodeList_append (NodeList * to, NodeList * from)
{
  if (!NodeList_is_empty (from))
    {
      NodeListIterator *itr = from->first;
      while (itr)
	{
	  NodeList_add (to, itr->value);
	  itr = itr->next;
	}
    }
}

void
NodeList_append_reverse_aux (NodeList * to, NodeListIterator * itr)
{
  if (itr->next)
    NodeList_append_reverse_aux (to, itr->next);
  NodeList_add (to, itr->value);
}

void
NodeList_append_reverse (NodeList * to, NodeList * from)
{
  if (!NodeList_is_empty (from))
    {
      NodeListIterator *itr = from->first;
      NodeList_append_reverse_aux (to, itr);
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
NodeList_join (NodeList * to, NodeList * from)
{
  if (NodeList_is_empty (to))
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
NodeList_find (NodeList * lst, LIST_TYPE elm)
{
  NodeListIterator *itr = lst->first;
  while (itr)
    {
      if (LIST_TYPE_EQUALS (itr->value, elm))
	return true;
      itr = itr->next;
    }
  return false;
}
