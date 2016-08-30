#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"

Graph *
Graph_init (uint size)
{
  int i;
  Graph *g = malloc (sizeof (Graph));
  g->size = size;
  g->edges = malloc (sizeof (NodeList *) * size);
  for (i = 0; i < size; i++)
    {
      g->edges[i] = NodeList_init ();
    }
  return g;
}

Graph *
Graph_init_random (uint size, uint modulus)
{
  Graph *g = Graph_init (size);
  // generate random edges, each edge with probability 1/modulus
  srand (time (NULL));
  uint i, j;
  for (i = 0; i < size; i++)
    for (j = i + 1; j < size; j++)
      if (rand () % modulus == 0)
	{
	  Graph_add_edge (g, i, j);
	}
  return g;
}

void
Graph_delete (Graph * g)
{
  int i;
  for (i = 0; i < g->size; i++)
    {
      NodeList_delete (g->edges[i]);
    }
  free (g->edges);
  free (g);
}

void
Graph_add_edge (Graph * g, uint i, uint j)
{
  // TODO: eventually assert there are no duplicates
  NodeList_add (g->edges[i], j);
  NodeList_add (g->edges[j], i);
}

// useful when certain generators number from 1 to N instead of from 0 to N-1
void
Graph_add_edge_ (Graph * g, uint i, uint j)
{
  Graph_add_edge (g, i - 1, j - 1);
}

NodeListIterator *
Graph_neighbours (Graph * g, uint v)
{
  return g->edges[v]->first;
}

Graph *
Graph_init_file (char *filename)
{
  FILE *fp;
  fp = fopen (filename, "r");

  uint size, numedge;

  fscanf (fp, "p edge %d %d\n", &size, &numedge);

  Graph *g = Graph_init (size);
  char line[30];
  uint i, j, tmp;
  while ((fscanf (fp, "e %i %i %i\n", &i, &j, &tmp)) != EOF)
    {
      Graph_add_edge (g, i - 1, j - 1);
    }

  fclose (fp);

  return g;
}

Graph *
Graph_init_file_simple (char *filename)
{
  FILE *fp;
  fp = fopen (filename, "r");

  uint size;

  fscanf (fp, "%d\n\n", &size);

  Graph *g = Graph_init (size);

  char line[10];
  uint i, j;
  while ((fscanf (fp, "%u %[-] %u\n", &i, line, &j)) != EOF)
    {
      Graph_add_edge (g, i, j);
    }

  fclose (fp);

  return g;
}

/**
 * GENERATES A CHAIN OF TRIANGLES CONNECTED AT ONE VERTEX
 EXAMPLE:
 1--------4       7       10--------13
 º \      º \     º \     º \       º  \
      º   3    º   6   º   9---+---12    º    15
 º /      º /     º /     º /       º  /
 2        5-------8       11        14

 DATE:         7-1-91
 LANGUAGE:     FORTRAN 77
 PROGRAMMERS:  N. RITCHEY
 B. MATTINGLY
 YOUNGSTOWN STATE UNIVERSITY

 * @param k Number of triangles
 * @return a graph with 3*k vertices and 4*k-1 edges that is a chain of triangles.
 *
 * Converted from fortran with the help of f2c.
 */
Graph *
Graph_init_triangles (uint k)
{
  Graph *g = Graph_init (3 * k);
  // generate random edges, each edge with probability 1/2

  uint n, m, i1, i__, i__1, i2, l;

  n = k * 3;
  m = (k << 2) - 1;
  l = 1;

L5:i1 = (l - 1) * 3 + 1;
  i2 = i1 + 1;
  i__1 = i2;
  for (i__ = i1; i__ <= i__1; ++i__)
    {
      Graph_add_edge_ (g, i__, i__ + 1);
      if (i__ < i2)
	Graph_add_edge_ (g, i__, i__ + 2);
    }

  if (l < k)
    {
      ++l;
      goto L5;
    }

  i__ = 0;
  m = 0;

  i__1 = k - 1;
  for (l = 1; l <= i__1; ++l)
    {
      m = (m + 1) % 3;
      if (m == 1)
	{
	  ++i__;
	}
      else
	{
	  i__ += 4;
	}
      Graph_add_edge_ (g, i__, i__ + 3);
    }
  return g;
}

/**   GENERATES A CHAIN OF TRIANGLES CONNECTED AT ALL VERTICES
 EXAMPLE:
 1--------4-------7-------10--------13
 º \      º \     º \     º \       º  \
 º   3----+---6---+---9---+---12----+----15
 º /      º /     º /     º /       º  /
 2--------5-------8-------11--------14

 DATE:         7-1-91
 LANGUAGE:     FORTRAN 77
 PROGRAMMERS:  N. RITCHEY
 B. MATTINGLY
 YOUNGSTOWN STATE UNIVERSITY

 INPUT:  K = NUMBER OF TRIANGLES

 OUTPUT:  NUMBER OF VERTICES = N = 3*K
 NUMBER OF EDGES    = M = 6*K - 3

 Converted from fortran with the help of f2c.
 */
Graph *
Graph_init_triangles2 (uint k)
{
  Graph *g = Graph_init (3 * k);

  uint n, m, i1, i__, i__1, i2, l;

  n = k * 3;
  m = k * 6 - 3;

  l = 1;
L5:i1 = (l - 1) * 3 + 1;
  i2 = i1 + 1;

  i__1 = i2;
  for (i__ = i1; i__ <= i__1; ++i__)
    {
      Graph_add_edge_ (g, i__, i__ + 1);
      if (i__ < i2)
	Graph_add_edge_ (g, i__, i__ + 2);
    }

  if (l < k)
    {
      ++l;
      goto L5;
    }

  i__1 = n - 3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      Graph_add_edge_ (g, i__, i__ + 3);
    }

  return g;
}

/**     PROGRAM GABOW


 REFERENCE:   H. N. GABOW, "AN EFFICIENT IMPLEMENTATION OF EDMONDS'
 ALGORITHM FOR MAXIMUM MATCHING ON GRAPHS", JACM 23
 (1976), PP. 221-234.

 INPUT:       NONE.  SET THE VALUE OF K IN THE PARAMETER STATEMENT.

 OUTPUT:      NUMBER OF VERTICES = N = 6*K
 NUMBER OF EDGES    = M = 8*K*K
 LIST OF EDGES (ALL EDGE COSTS ARE SET TO 1)

 THE GRAPH IS CONSTRUCTED SO THAT VERTICES 1-4K FORM A
 COMPLETE SUBGRAPH.  FOR I = 1 TO 2K, VERTEX (2I-1) IS
 JOINED TO VERTEX 4K+1.

 GABOW HAS SHOWN THAT SUCH A GRAPH WILL BE HARD FOR EDMOND'S
 CARDINALITY MATCHING ALGORITHM.

 PROGRAMMER:  R. BRUCE MATTINGLY
 LANGUAGE:    FORTRAN 77
 DATE:        FEBRUARY 28, 1991

 Converted from fortran with the help of f2c.
*/

Graph *
Graph_init_hardcard (uint k)
{
  Graph *g = Graph_init (6 * k);

  uint n, m, k4, k4p, k4m, i__, i__1, i__2, j;

  n = k * 6;
  m = (k << 3) * k;
  k4 = k << 2;
  k4p = k4 + 1;
  k4m = k4 - 1;

  i__1 = k4m;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      i__2 = k4;
      for (j = i__ + 1; j <= i__2; ++j)
	{
	  Graph_add_edge_ (g, i__, j);
	}
      if (i__ % 2 == 1)
	{
	  j = k4 + (i__ + 1) / 2;
	  Graph_add_edge_ (g, i__, j);
	}
    }

  return g;
}
