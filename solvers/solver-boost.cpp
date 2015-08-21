//=======================================================================
// Copyright (c) 2005 Aaron Windsor
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//=======================================================================
#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <stdio.h>

#include <boost/graph/max_cardinality_matching.hpp>
extern "C" {
#include "../src/DIMACS.h"
}


using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> my_graph; 




struct graph_data {
  uint num_vertices, num_edges;
  my_graph *g;
};

void *_BOOST_init_file_init_graph ( uint num_vertices, uint num_edges )
{
    struct graph_data *data = new struct graph_data;
    my_graph *g = new my_graph(num_vertices);
    data->g = g;
    data->num_vertices = num_vertices;
    data->num_edges = num_edges;
    return data;
}

void _BOOST_init_file_add_edge ( void *d, uint i, uint j )
{
  struct graph_data *data = (struct graph_data *) d;
  add_edge (i - 1, j - 1, *(data->g));
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

  struct graph_data *data = (struct graph_data *) _DIMACS_read_graph ( fp, &_BOOST_init_file_init_graph, &_BOOST_init_file_add_edge, &success, message );

  my_graph *g = data->g;
  uint size = data->num_vertices;
  
  if ( g == 0 ) {
        printf ( "%s", message );
        return -1;
    }
  fclose (fp);
  
  // our vertices are stored in a vector, so we can refer to vertices
  // by integers in the range 0..15

  std::vector<graph_traits<my_graph>::vertex_descriptor> mate(size);

  // find the maximum cardinality matching. we'll use a checked version
  // of the algorithm, which takes a little longer than the unchecked
  // version, but has the advantage that it will return "false" if the
  // matching returned is not actually a maximum cardinality matching
  // in the graph.

  edmonds_maximum_cardinality_matching(*g, &mate[0]);

  // std::cout << std::endl << "Found a matching of size " << matching_size(g, &mate[0]) << std::endl;

  // std::cout << "The matching is:" << std::endl;

  int matchnum = 0;
  graph_traits<my_graph>::vertex_iterator vi, vi_end;
  for(boost::tie(vi,vi_end) = vertices(*g); vi != vi_end; ++vi)
    if (mate[*vi] != graph_traits<my_graph>::null_vertex() && *vi < mate[*vi]) {
      // std::cout << "{" << *vi << ", " << mate[*vi] << "}" << std::endl;
      matchnum++;
    }

  std::cout << matchnum;
  std::cout << std::endl;

  return 0;
}  
