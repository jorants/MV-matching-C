

#ifndef LIBMV_H
#define LIBMV_H

/**
// Typical use of libmv. 

// Let's call "G" to the graph you are interested in knowing the maximum cardinality matching of.

unsigned int size;

... do some stuff ...
... until size == number of vertices in G ...

Graph *graph = Graph_init(size)

... make sure you give a number from 0 to size-1 to each vertex in G ...
... then do some loop or something, and for each edge (v1, v2) in G ...
... call the function: ...
   Graph_add_edge(graph, v1, v2);

EdgeList *lst = MV_MaximumCardinalityMatching(graph);

// at this point, lst is a list of edges containing a maximum cardinality matching
// to get the individual edges in this list, use an iterator, as follows:
EdgeListIterator *itr = lst->first;
while (itr != 0) {
	Edge edge = itr->value;
	
	... do stuff with edge, maybe store it in your own data structure or whatever ...
	
	itr = itr->next;
}

// To clear up the memory that was allocated, call:
EdgeList_delete(lst);
Graph_delete(graph);
*/


typedef struct {} Graph;

typedef struct
{
  unsigned int v1, v2;
} Edge;

typedef struct EdgeListIterator
{
  Edge value;
  struct EdgeListIterator *next;
} EdgeListIterator;

typedef struct
{
  EdgeListIterator *first, *last;
} EdgeList;

Graph *Graph_init (unsigned int size);
Graph *Graph_init_file (char *filename);
Graph *Graph_init_file_simple (char *filename);
Graph *Graph_init_triangles (uint k);
Graph *Graph_init_triangles2 (uint k);
Graph *Graph_init_hardcard (uint k);

void Graph_add_edge (Graph * g, unsigned int v1, unsigned int v2);


void EdgeList_delete (EdgeList * lst);
void Graph_delete (Graph * g);

/**
 * Given a graph g, returns a maximum cardinality matching for g.
 * @return An EdgeList instance; the edges in the list form a maximum-cardinality matching.
 * @param g A graph.
 */
EdgeList *MV_MaximumCardinalityMatching (Graph * g);

#endif // LIBMV_H
