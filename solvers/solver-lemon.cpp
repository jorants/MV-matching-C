#include <string>
#include <iostream>

#include <cassert>
#include <stdio.h>



#include <lemon/matching.h>
#include <lemon/smart_graph.h>
#include <lemon/concepts/graph.h>
#include <lemon/concepts/maps.h>
#include <lemon/lgf_reader.h>
#include <lemon/math.h>

using namespace std;
using namespace lemon;

GRAPH_TYPEDEFS(SmartGraph);


extern "C" {
#include "../src/DIMACS.h"
}





struct graph_data {
  uint num_vertices, num_edges;
  Node *nodes;
  SmartGraph *g;
};



void *_LEMON_init_file_init_graph ( uint num_vertices, uint num_edges )
{
    struct graph_data *data = new struct graph_data;
    data->num_vertices = num_vertices;
    data->num_edges = num_edges;
    SmartGraph *g = new SmartGraph();
    data->nodes = new Node[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
      data->nodes[i] = g->addNode();
    }
    data->g = g;
    return data;
}

void _LEMON_init_file_add_edge ( void *d, uint i, uint j )
{
  struct graph_data *data = (struct graph_data *) d;
  data->g->addEdge(data->nodes[i-1], data->nodes[j-1]);
}



int main(int argc, char *argv[])
{

  // Create the following graph: (it'll look better when output
  // to the terminal in a fixed width font...)

  // It has a perfect matching of size 8. There are two isolated
  // vertices that we'll use later...


  if (argc != 2) {
    printf("Usage: %s <name of file in DIMACS format>\n", argv[0]);
    exit(-1);
  }

  char *filename = argv[1];
  
  // printf("Reading file: %s.\n", filename);
  
  FILE *fp;
  fp = fopen (filename, "r");
  char message[1024];
  bool success;

  struct graph_data *data = (struct graph_data *) _DIMACS_read_graph ( fp, &_LEMON_init_file_init_graph, &_LEMON_init_file_add_edge, &success, message );

  SmartGraph *g = data->g;
  
  if ( g == 0 ) {
        printf ( "%s", message );
        return -1;
    }
  fclose (fp);
  
  clock_t start_time, finish_time;

  start_time = clock();

  MaxMatching<SmartGraph> mm(*g);  
  mm.startSparse();
  mm.matchingSize();
  
//    perfect = 2 * mm.matchingSize() == countNodes(graph);
  
  finish_time = clock();

  std::cout << mm.matchingSize();
  std::cout << std::endl;
  printf("The algorithm took %.3f seconds.\n", (double)(finish_time - start_time) / CLOCKS_PER_SEC);

  return 0;
}  
