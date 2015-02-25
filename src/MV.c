#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "MV.h"


uint step = 0;
char filename[200];

char *
currStepPrintFile (char *path)
{
  sprintf (filename, "%s/%03d.dot", path, step);
  step++;
  return filename;
}


/**
 * @return Returns true iff the i-j edge is matched
 */
bool
MV_edge_matched (MVInfo * mvi, uint i, uint j)
{
  uint i_match = mvi->v_info[i]->matched;
  return i_match == j;
}

/**
 * Computes the tenacity of a given edge.
 */
uint
MV_bridge_tenacity (MVInfo * mvi, uint i, uint j)
{

  uint tenacity;
  if (MV_edge_matched (mvi, i, j))	// if we are processing a matched edge
    tenacity = mvi->v_info[i]->oddlevel + mvi->v_info[j]->oddlevel + 1;
  else
    tenacity = mvi->v_info[i]->evenlevel + mvi->v_info[j]->evenlevel + 1;
  return tenacity;
}


/**
 * Processes the given edge in a BFS-search. See implementation for details.
 */
void
MV_BFS_step_process_edge (MVInfo * mvi, uint i, uint j)
{
  uint level = mvi->level;
  uint neighbourMinlevel = VertexInfo_min_level (mvi->v_info[j]);
  uint neighbourParityLevel = (level % 2 == 0 ?
			       mvi->v_info[j]->evenlevel : mvi->
			       v_info[j]->oddlevel);

  // if j has a smaller min-level than level(i) + 1, we mark the i-j edge as bridge
  if (neighbourMinlevel <= level)
    {
      // if the neighbour already has the appropriate
      // level (if it is not infinity)
      // then we can know the tenacity of this bridge
      // as soon as we mark it as a bridge;
      if (neighbourParityLevel < INFINITY)
	{
	  // if our neighbour's level (of the same parity) is strictly smaller,
	  // or if it is the same level as us, but our neighbour has precedence,
	  // then we have already scanned this edge and marked it as a bridge
	  // when we were doing BFS on it (on the neighbour).
	  if (neighbourParityLevel < level
	      || (neighbourParityLevel == level && j < i))
	    {
	    }
	  // otherwise, we have precedence and we mark the edge as bridge ourselves
	  // by adding it to the list of bridges of appropriate tenacity
	  else
	    {
	      Edge e = {.v1 = i,.v2 = j };
	      uint tenacity_minus_one = MV_bridge_tenacity (mvi, i, j) - 1;
	      EdgeList_add (mvi->bridges[tenacity_minus_one / 2], e);

	    }
	}
      // If the neighbour still doesn't have the level required to compute
      // this edge's tenacity, then that level will be a maxlevel
      // and we need to wait until that maxlevel is attributed in a DDFS
      // in order to know the bridge's tenacity
      else
	{
	  // so we set a "reminder" so that when j is given its maxlevel
	  // in the DDFS, it also adds the bridge to the list of bridges
	  // of appropraite tenacity
	  NodeList_add (mvi->v_info[j]->hanging_bridges, i);
	}

    }
  // j already has minlevel and it equals level(i)+1, then i is another predecessor of j
  else if (neighbourMinlevel == level + 1)
    {
      NodeList_add (mvi->v_info[j]->predecessors, i);
    }
  // if j has no minlevel yet, then i is the first predecessor of j
  // we then store level(i)+1 as j's minlevel, and add j
  // to the list of nodes having such a minlevel
  else if (neighbourMinlevel > level + 1)
    {
      if (level % 2 == 0)	// level is even
	mvi->v_info[j]->oddlevel = level + 1;
      else
	mvi->v_info[j]->evenlevel = level + 1;

      NodeList_add (mvi->v_info[j]->predecessors, i);

      NodeList_add (mvi->levels[level + 1], j);
    }
}



/**
 * Does one step of the breadth-first-search. At even levels, unmatched edges are transversed, and at odd levels,
 * the (single) matched edge is transversed. MV_BFS_step_process_edge is called on each transversed edge.
 */
void
MV_BFS_step (MVInfo * mvi)
{

  uint level = mvi->level;
  NodeListIterator *itr = mvi->levels[level]->first;

  // traverse all unmatched edges of all nodes in current level
  if (level % 2 == 0)
    {
      while (itr)
	{
	  uint i = itr->value;
	  uint matched = mvi->v_info[i]->matched;
	  NodeListIterator *neighbourItr = mvi->graph->edges[i]->first;
	  while (neighbourItr)
	    {
	      uint j = neighbourItr->value;
	      if (matched != j)
		MV_BFS_step_process_edge (mvi, i, j);
	      neighbourItr = neighbourItr->next;
	    }
	  itr = itr->next;
	}
    }
  // traverse all matched edges of all nodes in current level
  else
    {
      while (itr)
	{
	  uint i = itr->value;
	  uint j = mvi->v_info[i]->matched;
	  if (j != UNMATCHED)
	    MV_BFS_step_process_edge (mvi, i, j);
	  itr = itr->next;
	}
    }

}


/**
 * Given a node, finds and returns base*(node), where base*(n) = n if n is a node that is not in a petal, and 
 * base*(n) = base*(b) if n is a node which is in a petal with base b.
 */
uint
DDFS_iterated_base (MVInfo * mvi, uint node)
{

  if (mvi->pathc == 2){
      //uint * linkholder = mvi->links+node;
      uint top = node;
      uint * linkholder =   mvi->v_info[top]->petal->linkholder ? mvi->v_info[top]->petal->linkholder :  mvi->links+node;

      Petal *p;
      while (top != BLOSOM_DELETED && (p = mvi->v_info[top]->petal) != 0){
	if (p->linkholder != NULL)
	  top = *p->linkholder;
	else
	  top = p->base;
        p->linkholder = linkholder;
      }
      *linkholder = top;
      return top;

    }else if(mvi->pathc)
    {
      uint top = node;
      Petal *p;
      while (top != BLOSOM_DELETED && (p = mvi->v_info[top]->petal) != 0)
	if (p->link != INFINITY)
	  top = p->link;
	else
	  top = p->base;

      uint v = node;
      uint vnext;

      while (v != top)
	{
	  p = mvi->v_info[v]->petal;
	  if (p->link != INFINITY)
	    vnext = p->link;
	  else
	    vnext = p->base;
	  p->link = top;
	  v = vnext;
	}
      return top;
    }
  else
    {
      uint top = node;
      Petal *p;
      while (top != BLOSOM_DELETED && (p = mvi->v_info[top]->petal) != 0)
	top = p->base;
      return top;
    }

}


// only used 200 times in 5000 random graphs, not worth to change with path compression
// WRONG! NOTE: CAN NOT use path compression here, because the iteration does not allways go to the end

/**
 * Computes base*(node) up to, but excluding base (if node != base).
 */
uint
DDFS_iterated_base_up_to (MVInfo * mvi, uint node, uint base)
{

  uint top = node;
  if (node == base)
    return node;
  Petal *p;
  while ((p = mvi->v_info[top]->petal) != 0 && p->base != BLOSOM_DELETED
	 && p->base != base)
    {
      top = p->base;
    }
  return top;
}

/**
 * @return Returns true iff base is an iterated base of node.
 */
bool
DDFS_iterates_to_base (MVInfo * mvi, uint node, uint base)
{

  uint top = node;
  if (node == base)
    return true;
  Petal *p;
  while ((p = mvi->v_info[top]->petal) != 0 && p->base != BLOSOM_DELETED)
    {
      top = p->base;
      if (top == base)
	return true;
    }
  return false;
}

/**
 * Sets the ddfs predecessor of \a vertex to \a predecessor;
 * if \a predecessor belongs to a petal, then
 * the ddfs path is \a predecessor -- base-of-petal -- \a vertex.
 * if the predecessor of \a vertex is already set, then it
 * does nothing (so only the first predecessor counts).
 * @param mvi
 * @param vertex
 * @param predecessor
 * @param entry
 */
void
DDFS_set_predecessor (MVInfo * mvi, uint vertex, uint predecessor)
{
  if (mvi->v_info[vertex]->ddfs_predecessor == INFINITY)
    {
      mvi->v_info[vertex]->ddfs_predecessor = predecessor;
      // mvi->v_info[vertex]->ddfs_entry = entry;
    }
}

/**
 * Finds the next alternative path for a given stack.
 * @param mvi
 * @param stack
 * @param numUnexausted
 * @return
 */ bool
DDFS_backtrack (MVInfo * mvi, PointerList * stack, uint * numUnexausted)
{
  if (*numUnexausted == 0)
    return false;

  NodeListIterator *itr, *brother;

  // find first stack element that has a brother
  // (i.e. first stack element that isn't the last non-deleted
  // predecessor)
  while ((*numUnexausted) > 0)
    {
      itr = PointerList_top (stack);

      brother = itr->next;
      if (brother == NULL)
	PointerList_pop (stack);
      else
	{
	  // look for first non-deleted brother
	  while (brother
		 && DDFS_iterated_base (mvi, brother->value)
		 == BLOSOM_DELETED)
	    brother = brother->next;

	  // if none was found
	  if (brother == NULL)
	    {
	      // move up the stack
	      PointerList_pop (stack);
	      // we exausted the brothers of this stack element
	      (*numUnexausted)--;
	    }
	  // if we found a non-deleted brother (we got what we wanted)
	  else
	    {
	      // if it also turns out to be the last brother (colored or not)
	      if (brother->next == NULL)
		(*numUnexausted)--;
	      break;
	    }
	}
    }

  if (brother == NULL)
    {
      PointerList_erase (stack);
      return false;
    }

  // set new top of stack to the next brother of such element
  PointerList_set_top (stack, brother);

  // set dfs predecessor
  assert (!PointerList_is_empty_or_singleton (stack));
  NodeListIterator *parent_itr = PointerList_nth (stack, 1);
  uint parent_entry = parent_itr->value;
  uint parent = DDFS_iterated_base (mvi, parent_entry);

  // sets DDFS predecessor of the brother
  DDFS_set_predecessor (mvi, brother->value, parent);

  if (mvi->output)
    {
      // color the outgoing edge
      Edge e;

      e.v1 = parent;
      e.v2 = itr->next->value;

      if (mvi->v_info[e.v1]->ddfs_color == red)
	EdgeList_add (mvi->redEdges, e);
      else
	EdgeList_add (mvi->blueEdges, e);

      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
    }
  return true;

}

/**
 * Sets the stack to be a path to the next uncolored node in DFS ordering.
 * @param mvi
 * @param stack
 * @param numUnexausted
 */
bool
DDFS_next_node (MVInfo * mvi, PointerList * stack, uint * ptr_numUnexausted)
{
  NodeListIterator *stackTop = PointerList_top (stack);
  uint entry = stackTop->value;
  uint top = DDFS_iterated_base (mvi, entry);

  // add first DFS predecessor to stack
  NodeListIterator *predecessor_itr = mvi->v_info[top]->predecessors->first;

  NodeListIterator bogus_itr = {.value = INFINITY,.next = predecessor_itr };

  PointerList_push (stack, &bogus_itr);
  (*ptr_numUnexausted)++;
  return DDFS_backtrack (mvi, stack, ptr_numUnexausted);


}

/**
 * Changes the given node's collor, adds the node to the given petal, and set's the node's ddfs-predecessor...
 * assuming that the node has not been coloured before. If it has, it does nothing.
 */
void
DDFS_set_node_info (MVInfo * mvi, uint node, DDFSColor color, Petal * petal,
		    uint ddfs_entry)
{
  // set the node's information if necessary
  if (mvi->v_info[node]->ddfs_color == none)
    {
      mvi->v_info[node]->ddfs_color = color;
      NodeList_add (petal->support, node);
      mvi->v_info[node]->ddfs_entry = ddfs_entry;
    }
}

/**
 * Backtracks along the ddfs-stack of the color new_route_color.
 * center should be the bottleneck (the node that both red and blue
 * are looking to go down through).
 */
bool
DDFS_find_new_route (MVInfo * mvi, DDFSInfo * ddfsi, uint center,
		     DDFSColor new_route_color)
{
  uint centerLvl = VertexInfo_min_level (mvi->v_info[center]);

  PointerList **startingStack =
    (new_route_color == blue ? &ddfsi->bStack : &ddfsi->rStack);
  uint *numUnexaustedPtr =
    (new_route_color ==
     blue ? &ddfsi->bNumUnexausted : &ddfsi->rNumUnexausted);
  uint myNumUnexausted = *numUnexaustedPtr;

  PointerList *myStack = PointerList_init ();
  PointerList_append (myStack, *startingStack);

  while (true)
    {
      NodeListIterator *stackTop = PointerList_top (myStack);
      uint entry = stackTop->value;
      uint top = DDFS_iterated_base (mvi, entry);
      uint topLvl = VertexInfo_min_level (mvi->v_info[top]);
      uint topColor = mvi->v_info[top]->ddfs_color;

      if (topColor == none && topLvl <= centerLvl && top != center)
	{
	  // successfully found new route!
	  PointerList_delete (*startingStack);
	  *startingStack = myStack;
	  *numUnexaustedPtr = myNumUnexausted;
	  return true;
	}
      else if (topColor == none && topLvl > centerLvl)
	{
	  // go down!
	  DDFS_set_node_info (mvi, top, new_route_color, ddfsi->petal, entry);

	  // find the next undeleted node in ddfs ordering
	  if (!DDFS_next_node (mvi, myStack, &myNumUnexausted))
	    break;

	}
      else if (top == center || topColor != none)
	{
	  // backtrack!
	  if (!DDFS_backtrack (mvi, myStack, &myNumUnexausted))
	    break;

	}

    }
  PointerList_delete (myStack);
  return false;
}

uint
DDFS_top (MVInfo * mvi, DDFSInfo * ddfsi, DDFSColor color)
{
  return DDFS_iterated_base (mvi, DDFSInfo_top (ddfsi, color));
}

bool
DDFS_node_deleted (MVInfo * mvi, unsigned int node)
{
  return DDFS_iterated_base (mvi, node) == BLOSOM_DELETED;
}

DDFS_close_petal (MVInfo * mvi, Petal * petal, uint tenacity, uint base,
		  bool update_lists)
{
  NodeListIterator *itr = petal->support->first;
  uint node;
  while (itr)
    {
      node = itr->value;
      VertexInfo *vi = mvi->v_info[node];
      vi->petal = petal;
      VertexInfo_set_tenacity (vi, tenacity);
      uint maxlevel = tenacity - VertexInfo_min_level (vi);

      // add node to appropriate level list, and
      // add hanging bridges to the list for appropriate tenacity
      if (update_lists)
	{
	  NodeList_add (mvi->levels[maxlevel], node);

	  // now that we attributed the maxlevel, we should be able to
	  // attribute tenacity to any edges previously marked as
	  // bridges, but still waiting for this node's maxlevel
	  NodeListIterator *itr_hanging_bridges = vi->hanging_bridges->first;
	  while (itr_hanging_bridges)
	    {
	      Edge e = {.v1 = itr_hanging_bridges->value,.v2 = node };
	      uint tenacity_minus_one = MV_bridge_tenacity (mvi, e.v1, e.v2)
		- 1;
	      EdgeList_add (mvi->bridges[tenacity_minus_one / 2], e);

	      itr_hanging_bridges = itr_hanging_bridges->next;
	    }
	}

      itr = itr->next;
    }

  petal->base = base;
  //  petal->linkholder = mvi->links+node;
  //*(mvi->links+node) = base
}


/**
 * This implements the double-depth-first-search procedure, as defined by Micali and Vazirani.
 * @param bridge the bridge from where the ddfs starts.
 */
bool
MV_DDFS (MVInfo * mvi, Edge bridge)
{
  bool found = false;

  if (mvi->output)
    {
      printf ("Bridge: %d -- %d\n", bridge.v1, bridge.v2);
      mvi->currentDDFSBridge = bridge;
    }
  uint bluenode = bridge.v1;
  uint rednode = bridge.v2;

  if (DDFS_node_deleted (mvi, bluenode) || DDFS_node_deleted (mvi, rednode))
    return false;

  DDFSInfo *ddfsi = DDFSInfo_init (mvi->petals);

  // Prepare base case for top blue and red nodes

  ddfsi->petal->bluenode = bluenode;
  NodeListIterator blueitr = {.value = bluenode,.next = 0 };
  DDFSInfo_push (ddfsi, blue, &blueitr);

  ddfsi->petal->rednode = rednode;
  NodeListIterator reditr = {.value = rednode,.next = 0 };
  DDFSInfo_push (ddfsi, red, &reditr);


  while (true)
    {
      if (mvi->output)
	{
	  MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
	}
      uint btop_entry = DDFSInfo_top (ddfsi, blue);
      uint rtop_entry = DDFSInfo_top (ddfsi, red);

      uint btop = DDFS_iterated_base (mvi, btop_entry);
      uint rtop = DDFS_iterated_base (mvi, rtop_entry);

      if (btop == BLOSOM_DELETED)
	{
	  bool backtracked =
	    DDFS_backtrack (mvi, ddfsi->bStack, &ddfsi->bNumUnexausted);

	  if (backtracked)
	    continue;
	  else
	    break;
	}

      if (rtop == BLOSOM_DELETED)
	{
	  bool backtracked =
	    DDFS_backtrack (mvi, ddfsi->rStack, &ddfsi->rNumUnexausted);

	  if (backtracked)
	    continue;
	  else
	    break;
	}


      uint blvl = VertexInfo_min_level (mvi->v_info[btop]);
      uint rlvl = VertexInfo_min_level (mvi->v_info[rtop]);

      if (btop == rtop)		// backtrack if possible; if not we found bottleneck
	{

	  if (!DDFS_find_new_route (mvi, ddfsi, btop, blue))
	    {
	      if (!DDFS_find_new_route (mvi, ddfsi, rtop, red))
		{
		  // found bottleneck!!
		  uint tenacity = MV_bridge_tenacity (mvi, bluenode, rednode);

		  DDFS_close_petal (mvi, ddfsi->petal, tenacity, btop, true);

		  break;
		}
	    }
	}
      else if (blvl == 0 && rlvl == 0)	// found augmenting path?
	{
	  DDFS_set_node_info (mvi, btop, blue, ddfsi->petal, btop_entry);
	  DDFS_set_node_info (mvi, rtop, red, ddfsi->petal, rtop_entry);

	  uint tenacity = MV_bridge_tenacity (mvi, bluenode, rednode);

	  MV_Augment (mvi, ddfsi);

	  DDFS_close_petal (mvi, ddfsi->petal, tenacity, BLOSOM_DELETED,
			    false);

	  found = true;
	  break;
	}
      // go deeper!
      else
	{

	  // go down!
	  DDFSColor mycolor = (blvl >= rlvl ? blue : red);
	  uint mytop = (blvl >= rlvl ? btop : rtop);
	  uint mytop_entry = (blvl >= rlvl ? btop_entry : rtop_entry);

	  DDFS_set_node_info (mvi, mytop, mycolor, ddfsi->petal, mytop_entry);

	  bool result;
	  if (mycolor == blue)
	    result =
	      DDFS_next_node (mvi, ddfsi->bStack, &ddfsi->bNumUnexausted);
	  else
	    result =
	      DDFS_next_node (mvi, ddfsi->rStack, &ddfsi->rNumUnexausted);
	  // if we failed to find an undeleted next node, there is neither
	  // petal nor augmenting path going through this bridge
	  if (!result)
	    {
	      found = false;
	      break;
	    }

	}

    }

  DDFSInfo_delete (ddfsi);
  if (mvi->output)
    {
      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
      mvi->currentDDFSBridge.v1 = INFINITY;
      mvi->currentDDFSBridge.v2 = INFINITY;
    }

  return found;
}

/**
 * Given a path \a p, and a vertex \a v belonging to a \a petal,
 * appends to \a p the path from \a v to the bridge of \a petal,
 * including the bridge edge itself.
 */
void
MV_append_path_upto_bridge (MVInfo * mvi, EdgeList * p, uint v, Petal * petal)
{
  assert (mvi->v_info[v]->petal == petal);
  uint w = v;
  DDFSColor color = mvi->v_info[v]->ddfs_color;
  uint target = (color == blue ? petal->bluenode : petal->rednode);
  do
    {
      assert (mvi->v_info[w]->ddfs_predecessor != INFINITY);

      w = mvi->v_info[w]->ddfs_predecessor;

      Edge e = {.v1 = v,.v2 = w };
      EdgeList_add (p, e);

      v = w;
    }
  while (w != target);

  Edge e =
    {.v1 = w,.v2 = (color == blue ? petal->rednode : petal->bluenode) };
  EdgeList_add (p, e);
}

/**
 * Given a path \a p, and a vertex \a v belonging to a \a petal,
 * appends to \a p the (shortest) alternating path from \a v to the base of \a petal.
 */
void
MV_append_path_downto_base (MVInfo * mvi, EdgeList * p, uint v, Petal * petal)
{
  uint w = v;
  do
    {
      assert (!NodeList_is_empty (mvi->v_info[w]->predecessors));

      w = mvi->v_info[w]->predecessors->first->value;

      Petal *w_petal = mvi->v_info[w]->petal;
      if (w_petal != 0)		// if we must unfold a petal
	{
	  w = w_petal->base;
	}
      else
	{
	  Edge e = {.v1 = v,.v2 = w };
	  EdgeList_add (p, e);
	}

      v = w;
    }
  while (w != petal->base);
}

NodeList *MV_base_to_entry_path (MVInfo * mvi, uint base, uint entry,
				 uint parity);

/**
 * Given the VertexInfo for a given vertex that is part of some petal, returns 
 * the BFS-predecessor of that vertex which is part of the same petal.
 */
uint MV_get_predecessor_in_same_petal(MVInfo *mvi, VertexInfo *vi) {
  NodeListIterator *itr = vi->predecessors->first;
  
  if ( !itr->next )
    return itr->value;
  
  Petal *petal = vi->petal;
  
  while (itr) {
    uint predecessor = itr->value;
    if (DDFS_iterates_to_base(mvi, predecessor, petal->base)) {
      return predecessor;
    }
    
    itr = itr->next;
  }
  assert(0);
}


/**
 * Given a base of a petal (first_base) and an entry-point to that petal (i.e.,
 * a vertex which is part of that petal), returns a path from the base to the entry-point
 * of the given parity.
 */
NodeList *
MV_first_base_to_entry_path (MVInfo * mvi, uint first_base, uint entry,
			     uint parity)
{
  if (first_base == entry)
    {
      NodeList *path = NodeList_init ();

      NodeList_add (path, entry);
      return path;
    }
  // else:
  VertexInfo *entry_vi = mvi->v_info[entry];
  uint entry_min_level = VertexInfo_min_level (entry_vi);
  if (entry_min_level % 2 == parity % 2)	// go straight down
    {
      assert (!NodeList_is_empty (entry_vi->predecessors));

      uint predecessor = MV_get_predecessor_in_same_petal(mvi, entry_vi);
      uint predecessor_iterated_base = DDFS_iterated_base_up_to (mvi,
								 predecessor,
								 first_base);

      if (predecessor == predecessor_iterated_base)
	{
	  NodeList *p = MV_first_base_to_entry_path (mvi, first_base,
						     predecessor, parity + 1);
	  NodeList_add (p, entry);
	  return p;
	}
      else
	{
	  NodeList *p = MV_first_base_to_entry_path (mvi, first_base,
						     predecessor_iterated_base, 0);	// "0" for even
	  NodeList *p2 =
	    MV_base_to_entry_path (mvi, predecessor_iterated_base,
				   predecessor, parity + 1);

	  NodeList_pop(p2); // this prevents the predecessor_iterated_base node from appearing twice!
	  NodeList_join (p, p2);
	  NodeList_add (p, entry);
	  return p;
	}
    }
  else				// go up to the bridge, then straight down from the other bridge vertex to the base
    {
      DDFSColor color = entry_vi->ddfs_color;
      uint bridge_vertex = (color == blue ?
			    entry_vi->petal->bluenode : entry_vi->
			    petal->rednode);

      uint entry_entry = entry_vi->ddfs_entry;
      // if the entry vertex is itself an iterated base of some vertex
      // then we must go up from entry to entry_entry to pred
      if (entry != entry_entry)
	{
	  // if the entry_entry is the bridge vertex
	  // then the bridge must be an unmatched edge
	  if (entry_entry == bridge_vertex)
	    {
	      uint other_bridge_vertex = (color == blue ?
					  entry_vi->petal->rednode :
					  entry_vi->petal->bluenode);
	      //                            uint other_bridge_vertex_min_level = VertexInfo_min_level(
	      //                                            mvi->v_info[other_bridge_vertex]);

	      // p is set to the path straight from the base to
	      // the other side of the bridge; this path must be even
	      // because the bridge is unmatched
	      NodeList *p = MV_base_to_entry_path (mvi, first_base,
						   other_bridge_vertex, 0);

	      // and then we add a path from our side of the bridge (entry_entry)
	      // to our node (entry), which is also even,
	      // and which is reversed (so it goes from entry' = entry_entry to base' = entry)
	      NodeList *p2 = MV_base_to_entry_path (mvi, entry, entry_entry,
						    0);

	      // FIXME: don't reverse like this
	      NodeList *p2_reverse = NodeList_init ();
	      NodeList_append_reverse (p2_reverse, p2);
	      NodeList_delete (p2);

	      NodeList_join (p, p2_reverse);
	      return p;
	    }
	  // if we haven't yet reached the bridge vertex, we must
	  // go up the ddfs_predecessor chain
	  else
	    {
	      uint ddfs_pred = entry_vi->ddfs_predecessor;

	      // some even path p goes from base to predecessor,
	      NodeList *p = MV_first_base_to_entry_path (mvi, first_base,
							 ddfs_pred, 0);
	      // then we have an even path from entry_entry to entry
	      NodeList *p2 = MV_base_to_entry_path (mvi, entry, entry_entry,
						    0);

	      // FIXME: don't reverse like this
	      NodeList *p2_reverse = NodeList_init ();
	      NodeList_append_reverse (p2_reverse, p2);
	      NodeList_delete (p2);

	      NodeList_join (p, p2_reverse);
	      return p;
	    }
	}
      // if entry is a simple node (no petal needs unfolding)
      else
	{
	  // if that simple node happens to be one side of the bridge
	  if (entry == bridge_vertex)
	    {
	      uint other_bridge_vertex = (color == blue ?
					  entry_vi->petal->rednode :
					  entry_vi->petal->bluenode);
	      uint other_bridge_vertex_min_level =
		VertexInfo_min_level (mvi->v_info[other_bridge_vertex]);

	      // p is set to the path straight from the base to
	      // the other side of the bridge
	      // and then we add our vertex

	      VertexInfo *other_info = mvi->v_info[other_bridge_vertex];

	      uint other_bridge_ov_vertex = (other_info->ddfs_color == blue ?
					     other_info->petal->rednode :
					     other_info->petal->bluenode);
	      if (other_bridge_ov_vertex == entry)
		{
		  // p is set to the path straight from the base to
		  // the other side of the bridge
		  // and then we add our vertex

		  NodeList *p = MV_base_to_entry_path (mvi, first_base,
						       other_bridge_vertex,
						       other_bridge_vertex_min_level);
		  NodeList_add (p, bridge_vertex);	// bridge_vertex == entry
		  return p;
		}
	      else
		{
		  //other bridge after this:
		  NodeList *p = MV_base_to_entry_path (mvi, first_base,
						       other_bridge_vertex,
						       parity + 1);
		  NodeList_add (p, bridge_vertex);	// bridge_vertex == entry
		  return p;

		}

	    }
	  // if we haven't yet reached the bridge vertex, we must
	  // go up the ddfs_predecessor chain
	  else
	    {
	      uint ddfs_pred = entry_vi->ddfs_predecessor;

	      // some path p goes from base to predecessor,
	      NodeList *p = MV_first_base_to_entry_path (mvi, first_base,
							 ddfs_pred,
							 parity + 1);

	      // and then straight down to entry
	      NodeList_add (p, entry);
	      return p;
	    }

	}
    }
  return 0;
}


/**
 * Given an iterated base of a petal (base) and a vertex (entry-point) for which base is an iterated base,
 * returns a path from base to entry-point of the given parity.
 */
NodeList *
MV_base_to_entry_path (MVInfo * mvi, uint base, uint entry, uint parity)
{
  Petal *entry_petal = mvi->v_info[entry]->petal;

  if (entry_petal == 0 || entry_petal->base == base)
    {
      return MV_first_base_to_entry_path (mvi, base, entry, parity);
    }
  else
    {
      uint first_base = entry_petal->base;
      // remember that any el/ol(base, entry) path is the concatenation
      // of an el(base, first_base) and an el/ol(first_base, entry) path
      // so we take an odd path to the predecessor of first_base,
      // followed by an appropriate first_base to entry path
      uint first_base_pred =
	mvi->v_info[first_base]->predecessors->first->value;
      NodeList *p = MV_base_to_entry_path (mvi, base, first_base_pred, 1);
      NodeList *p2 = MV_first_base_to_entry_path (mvi, first_base, entry,
						  parity);

      NodeList_join (p, p2);
      return p;
    }
}

/**
 * Returns a path from the top to the bottom of the stack
 * @param mvi
 * @param stack A stack produced by DDFS
 * @return
 */
NodeList *
MV_unfold_stack (MVInfo * mvi, PointerList * stack)
{
  NodeList *path = NodeList_init ();

  PointerListIterator *sitr = stack->first;

  while (sitr)
    {
      NodeListIterator *selement = sitr->value;
      uint entry = selement->value;

      uint base = DDFS_iterated_base (mvi, entry);

      if (entry == base)
	{
	  NodeList_add (path, entry);
	}
      else
	{
	  NodeList *bepath = MV_base_to_entry_path (mvi, base, entry, 0);	// "0" for even
	  NodeList_join (path, bepath);
	}

      sitr = sitr->next;
    }
  return path;
}


/**
 * Given a DDFSInfo structure for a double-depth-first-search which succeeded in finding an augmenting path,
 * changes the current matching by augmenting along that path.
 * @param mvi Basic algorithm information
 * @param path A DDFSInfo structure for a successful ddfs
 */
void
MV_Augment (MVInfo * mvi, DDFSInfo * ddfsi)
{

  NodeList *blue_path = MV_unfold_stack (mvi, ddfsi->bStack);
  NodeList *red_path = MV_unfold_stack (mvi, ddfsi->rStack);

  // FIXME: don't reverse like this
  NodeList *red_path_reverse = NodeList_init ();
  NodeList_append_reverse (red_path_reverse, red_path);
  NodeList_delete (red_path);

  NodeList_join (blue_path, red_path_reverse);

  NodeListIterator *itr;

  if (mvi->output)
    {
      // EdgeList_erase(mvi->aug_path);
      itr = blue_path->first;
      while (itr)
	{
	  if (itr->next)
	    {
	      Edge e = {.v1 = itr->value,.v2 = itr->next->value };
	      EdgeList_add (mvi->aug_path, e);
	    }
	  itr = itr->next;
	}
      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
      EdgeList_erase (mvi->aug_path);
      printf ("Aug. Path: ");
    }

  // do the actual augmentation

  itr = blue_path->first;

  while (itr)
    {
      uint v1 = itr->value;

      itr = itr->next;
      assert (itr != 0);
      uint v2 = itr->value;
      if (mvi->output)
	printf ("%d, %d   ", v1, v2);

      mvi->v_info[v1]->matched = v2;
      mvi->v_info[v2]->matched = v1;

      itr = itr->next;
    }

  if (mvi->output)
    {
      printf ("\n");
      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
      EdgeList_erase (mvi->aug_path);
    }
  mvi->matched_num++;
  NodeList_delete (blue_path);


}

/**
 * Returns a maximal matching for the graph with the given MVInfo.
 * @param mvi Basic algorithm information
 */
void
MV_maximalMatching (MVInfo * mvi)
{
  // (do a maximal matching, why not...)
  uint n, i, j, k;
  NodeListIterator *itr;
  for (i = 0; i < mvi->graph->size; i++)
    {
      if (mvi->v_info[i]->matched == UNMATCHED)
	{
	  itr = mvi->graph->edges[i]->first;
	  while (itr)
	    {
	      j = itr->value;
	      if (mvi->v_info[j]->matched == UNMATCHED)
		{
		  mvi->v_info[i]->matched = j;
		  mvi->v_info[j]->matched = i;
		  break;
		}
	      itr = itr->next;
	    }
	}
    }
  NodeList_erase (mvi->levels[0]);
  for (i = 0; i < mvi->graph->size; i++)
    {
      if (mvi->v_info[i]->matched == UNMATCHED)
	NodeList_add (mvi->levels[0], i);
    }
}

EdgeList *
MV_MaximumCardinalityMatching_ (MVInfo * mvi)
{
  EdgeList *matching = EdgeList_init ();
  bool found = true;

  mvi->stage = 0;

  while (found && mvi->matched_num < mvi->graph->size/2)
    {				// for each stage
      found = false;
      for (mvi->level = 0; mvi->level < mvi->graph->size; mvi->level++)	// for each level
	{
	  MV_BFS_step (mvi);

	  if (mvi->output)
	    {
	      if (mvi->level == 0
		  || !NodeList_is_empty (mvi->levels[mvi->level]))
		MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
	    }
	  EdgeListIterator *eitr = mvi->bridges[mvi->level]->first;
	  while (eitr)
	    {
	      int i;
	      Edge e = eitr->value;
	      found = MV_DDFS (mvi, e) || found;
	      eitr = eitr->next;
	    }

	  if (found)
	    break;
	}
  //      if(!found)
      //printf("ended on %i,%i\n",mvi->stage,mvi->level);
      MVInfo_next_stage (mvi);
    }

  // Build matching.
  // goes through every node looking up its "matched",
  // and marks the "matched" node as "already checked"
  // so that we don't add edges twice
  uint i;
  uint checked[mvi->graph->size];
  memset (checked, 0, sizeof (uint) * mvi->graph->size);

  for (i = 0; i < mvi->graph->size; i++)
    {
      if (!checked[i])
	{
	  uint matched = mvi->v_info[i]->matched;
	  if (matched != UNMATCHED)
	    {
	      Edge e = {.v1 = i,.v2 = matched };
	      EdgeList_add (matching, e);
	      checked[matched] = true;
	    }
	}
    }

  return matching;
}

EdgeList *
MV_MaximumCardinalityMatching (Graph * g)
{
  MVInfo *mvi = MVInfo_init (g);

  if (mvi->output)
    {
      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
      MV_maximalMatching (mvi);
      MVInfo_next_stage (mvi);
      MVInfo_print_graphviz (mvi, currStepPrintFile (mvi->outpath));
      MVInfo_print_contents (mvi, "mv_progress/graph.txt");
    }

  EdgeList *matching = MV_MaximumCardinalityMatching_ (mvi);

  MVInfo_delete (mvi);
  return matching;
}
