/**
 @file
*/
#ifndef GRAPH_H
#define GRAPH_H

#include "basictypes.h"
#include "NodeList.h"


/**
 * Basic graph
 */
typedef struct
{
  //! number of nodes
  uint size;
  //! All the edges
  NodeList **edges;
} Graph;


Graph *Graph_init (uint size);
void Graph_delete (Graph * g);

void Graph_add_edge (Graph * g, uint v1, uint v2);
NodeListIterator *Graph_neighbours (Graph * g, uint v);

Graph *Graph_init_file (char *filename);
Graph *Graph_init_file_simple (char *filename);
Graph *Graph_init_triangles (uint k);
Graph *Graph_init_triangles2 (uint k);
Graph *Graph_init_hardcard (uint k);


Graph *Graph_init_random (uint size, uint modulus);


#endif // GRAPH_H
