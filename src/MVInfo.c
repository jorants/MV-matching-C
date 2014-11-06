#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MVInfo.h"

VertexInfo *
VertexInfo_init ()
{
  VertexInfo *vi = malloc (sizeof (VertexInfo));

  vi->ddfs_color = none;
  vi->predecessors = NodeList_init ();
  vi->petal = 0;
  vi->evenlevel = vi->oddlevel = INFINITY;
  vi->matched = UNMATCHED;
  vi->ddfs_predecessor = INFINITY;
  vi->ddfs_entry = INFINITY;

  vi->deleted = false;

  vi->hanging_bridges = NodeList_init ();
  return vi;
}

void
VertexInfo_delete (VertexInfo * vi)
{
  NodeList_delete (vi->predecessors);
  NodeList_delete (vi->hanging_bridges);
  free (vi);
}

void
VertexInfo_reset (VertexInfo * vi)
{
  vi->ddfs_color = none;
  NodeList_erase (vi->predecessors);
  vi->petal = 0;		// PETALS MUST BE ERASED ELSEWHERE
  vi->evenlevel = vi->oddlevel = INFINITY;
  vi->deleted = false;
  vi->ddfs_predecessor = INFINITY;
  vi->ddfs_entry = INFINITY;
  vi->deleted = false;
  // matched status does not change
}

bool
VertexInfo_has_min_level (VertexInfo * vi)
{
  return VertexInfo_min_level (vi) != INFINITY;
}

uint
VertexInfo_min_level (VertexInfo * vi)
{
  return (vi->evenlevel < vi->oddlevel ? vi->evenlevel : vi->oddlevel);
}

void
VertexInfo_set_tenacity (VertexInfo * vi, uint tenacity)
{
  if (vi->evenlevel < vi->oddlevel)
    vi->oddlevel = tenacity - vi->evenlevel;
  else
    vi->evenlevel = tenacity - vi->oddlevel;
}

MVInfo *
MVInfo_init (Graph * g)
{
  uint i;
  MVInfo *mvi = malloc (sizeof (MVInfo));

  mvi->matched_num = 0;

  mvi->graph = g;
  mvi->v_info = malloc (sizeof (VertexInfo *) * g->size);

  mvi->links = malloc (sizeof (int) * g->size);
  
  for (i = 0; i < g->size; i++)
    mvi->v_info[i] = VertexInfo_init ();

  mvi->levels = malloc (sizeof (NodeList *) * g->size);

  for (i = 0; i < g->size; i++)
    mvi->levels[i] = NodeList_init ();

  for (i = 0; i < g->size; i++)
    NodeList_add (mvi->levels[0], i);	// Level 0 = unmatched nodes

  mvi->bridges = malloc (sizeof (EdgeList *) * g->size);

  for (i = 0; i < g->size; i++)
    mvi->bridges[i] = EdgeList_init ();

  mvi->petals = PointerList_init ();


  mvi->stage = -1;
  MVInfo_next_stage (mvi);
  mvi->level = 0;
  mvi->output = false;
  mvi->pathc = true;
  return mvi;
}

MVInfo *
MVInfo_init_output (Graph * g, char *path)
{
  MVInfo *mvi = MVInfo_init (g);
  mvi->redEdges = EdgeList_init ();
  mvi->blueEdges = EdgeList_init ();
  mvi->currentDDFSBridge.v1 = INFINITY;
  mvi->currentDDFSBridge.v2 = INFINITY;
  mvi->aug_path = EdgeList_init ();
  mvi->output = true;
  mvi->outpath = path;
  return mvi;
}

void
MVInfo_delete (MVInfo * mvi)
{
  uint i;

  Petal_deleteAll (mvi->petals);
  PointerList_delete (mvi->petals);

  for (i = 0; i < mvi->graph->size; i++)
    EdgeList_delete (mvi->bridges[i]);

  free (mvi->bridges);

  for (i = 0; i < mvi->graph->size; i++)
    NodeList_delete (mvi->levels[i]);

  free (mvi->levels);

  for (i = 0; i < mvi->graph->size; i++)
    VertexInfo_delete (mvi->v_info[i]);

  free (mvi->v_info);

  if (mvi->output)
    {
      // EdgeList_delete(mvi->oldBridges);
      EdgeList_delete (mvi->redEdges);
      EdgeList_delete (mvi->blueEdges);
      EdgeList_delete (mvi->aug_path);
    }
  free (mvi);
}

void
MVInfo_next_stage (MVInfo * mvi)
{
  uint i;

  // erase current level and next level node lists,
  // and set current level = unmatched nodes (= level 0 nodes)
  for (i = 0; i < mvi->graph->size; i++)
    NodeList_erase (mvi->levels[i]);
  for (i = 0; i < mvi->graph->size; i++)
    EdgeList_erase (mvi->bridges[i]);

  // reset node levels
  for (i = 0; i < mvi->graph->size; i++)
    {
      VertexInfo *vi = mvi->v_info[i];
      VertexInfo_reset (vi);
      vi->oddlevel = INFINITY;
      if (vi->matched == UNMATCHED)
	{
	  vi->evenlevel = 0;
	  NodeList_add (mvi->levels[0], i);
	}
      else
	vi->evenlevel = INFINITY;
    }

  Petal_deleteAll (mvi->petals);

  mvi->stage++;

}

void
MVInfo_print_contents (MVInfo * mvi, char *filename)
{
  FILE *fp;
  fp = fopen (filename, "w");

  uint i;
  NodeListIterator *itr;

  fprintf (fp, "%d\n\n", mvi->graph->size);

  for (i = 0; i < mvi->graph->size; i++)
    {
      itr = mvi->graph->edges[i]->first;
      while (itr)
	{
	  uint j = itr->value;
	  if (i < j)
	    {
	      if (mvi->v_info[i]->matched == j)
		fprintf (fp, "%d -- %d\n", i, j);
	      else
		fprintf (fp, "%d - %d\n", i, j);
	    }
	  itr = itr->next;
	}
    }

  fclose (fp);
}


void
MVInfo_print_graphviz (MVInfo * mvi, char *filename)
{
  FILE *fp;
  fp = fopen (filename, "w");

  fprintf (fp, "strict graph\n{\n");

  fprintf (fp, "forcelabels=true;  rankdir=LR;");
  fprintf (fp, "lvlnode [shape=plaintext,label=\"stage = %d, level = %d\"];",
	   mvi->stage, mvi->level);


  uint i;
  NodeListIterator *itr;

  for (i = 0; i < mvi->graph->size; i++)
    {

      fprintf (fp, "n%03d [", i);

      fprintf (fp,
	       "shape=circle,style=filled,width=0.1,height=0.1,fixedsize=true");

      fprintf (fp, ",color=");

      DDFSColor color = mvi->v_info[i]->ddfs_color;

      if (color == none)
	fprintf (fp, "gray");
      else if (color == blue)
	fprintf (fp, "blue");
      else if (color == red)
	fprintf (fp, "red");

      fprintf (fp, ",label=\"%d\",labelfontsize=\"2\"", i);

      uint matched = mvi->v_info[i]->matched;
      if (matched == UNMATCHED)	// make unmatched nodes into circles
	fprintf (fp,
		 ",shape=circle,style=solid,color=black,width=0.2,height=0.2,fixedsize=true");
      if (!PointerList_is_empty (mvi->petals))	// make bases of petals into circles
	{
	  PointerListIterator *itr_petalbase = mvi->petals->first;

	  while (itr_petalbase)
	    {
	      Petal *p = itr_petalbase->value;
	      if (p->base == i)
		{
		  if (matched == UNMATCHED)
		    fprintf (fp, ",shape=square");
		  else
		    fprintf (fp,
			     ",shape=square,style=solid,color=black,width=0.1,height=0.1,fixedsize=true");

		}
	      itr_petalbase = itr_petalbase->next;
	    }
	}
      else
	{

	}

      uint el = mvi->v_info[i]->evenlevel;
      uint ol = mvi->v_info[i]->oddlevel;

      fprintf (fp, ",fontsize=\"6\", xlabel=\"(");
      if (el == INFINITY)
	fprintf (fp, "∞,");
      else
	fprintf (fp, "%d,", el);
      if (ol == INFINITY)
	fprintf (fp, "∞)\"");
      else
	fprintf (fp, "%d)\"", ol);

      fprintf (fp, "];\n");

    }

  for (i = 0; i < mvi->graph->size; i++)
    {
      uint matched = mvi->v_info[i]->matched;
      itr = mvi->graph->edges[i]->first;
      while (itr)
	{
	  uint j = itr->value;

	  if (j > i)
	    {

	      fprintf (fp, "n%03d -- n%03d", i, j);
	      fprintf (fp, "[");

	      if (NodeList_find (mvi->v_info[j]->predecessors, i))
		{
		  fprintf (fp, "arrowsize=.3,arrowhead=vee,dir=back,");
		}
	      else if (NodeList_find (mvi->v_info[i]->predecessors, j))
		{
		  fprintf (fp, "arrowsize=.3,arrowhead=vee,dir=forward,");
		}

	      Edge e = {.v1 = i,.v2 = j };
	      uint t;
	      for (t = 0; t < mvi->graph->size; t++)
		{
		  if (EdgeList_find (mvi->bridges[t], e))
		    break;
		}
	      if (t < mvi->graph->size)
		{
		  if (EDGE_LIST_TYPE_EQUALS (e, mvi->currentDDFSBridge))
		    fprintf (fp, "label=\"-> b(%d) <-\",fontsize=\"6\",",
			     2 * t + 1);
		  else
		    fprintf (fp, "label=\"b(%d)\",fontsize=\"6\",",
			     2 * t + 1);
		}
	      else
		fprintf (fp, "label=\"    \",fontsize=\"6\",");

	      if (EdgeList_find (mvi->redEdges, e))
		fprintf (fp, "color=red,");
	      else if (EdgeList_find (mvi->blueEdges, e))
		fprintf (fp, "color=blue,");

	      if (EdgeList_find (mvi->aug_path, e))
		fprintf (fp, "penwidth=3.0,");

	      if (matched == j)
		fprintf (fp, "style=dotted");

	      fprintf (fp, "];\n");
	    }
	  itr = itr->next;
	}

    }


  fprintf (fp, "}\n");
  fclose (fp);
}



MVInfo *
MVInfo_init_file_simple (char *filename)
{
  FILE *fp;
  fp = fopen (filename, "r");

  uint size;

  fscanf (fp, "%d\n\n", &size);

  Graph *g = Graph_init (size);
  MVInfo *mvi = MVInfo_init (g);

  char line[10];
  uint i, j;
  while ((fscanf (fp, "%u %[-] %u\n", &i, line, &j)) != EOF)
    {
      Graph_add_edge (g, i, j);
      if (strlen (line) == 2)
	{
	  mvi->v_info[i]->matched = j;
	  mvi->v_info[j]->matched = i;
	}
    }
  mvi->stage = -1;
  mvi->output = false;
  MVInfo_next_stage (mvi);

  fclose (fp);

  return mvi;
}


MVInfo *
MVInfo_init_file (char *filename)
{
  FILE *fp;
  fp = fopen (filename, "r");

  uint size, numedge;

  fscanf (fp, "p edge %d %d\n", &size, &numedge);

  Graph *g = Graph_init (size);
  MVInfo *mvi = MVInfo_init (g);
  char line[30];
  uint i, j, tmp;
  while ((fscanf (fp, "e %i %i %i\n", &i, &j, &tmp)) != EOF)
    {
      Graph_add_edge (g, i - 1, j - 1);
    }
  mvi->stage = -1;
  mvi->output = false;
  MVInfo_next_stage (mvi);

  fclose (fp);

  return mvi;
}
