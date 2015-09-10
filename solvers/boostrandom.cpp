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

#include <sys/time.h>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> my_graph; 




int main(int argc, char *argv[])
{



  if (argc <3){
    printf("Not engough arguments\n Usage: %s [V] [p]\n",argv[0]);
  }
  
  int V = atoi(argv[1]);
  double p = atof(argv[2]);
  

  my_graph g(V);

  srandom(7); //temp, change this to the time!
  
  int a,b;
  for(a=0;a<V-1;a++)
  {
    for(b=a+1;b<V;b++)
      {
	if(((float)random())/(RAND_MAX) <= p)
	    add_edge (a, b, g);
      }
  }


  // our vertices are stored in a vector, so we can refer to vertices
  // by integers in the range 0..15

  std::vector<graph_traits<my_graph>::vertex_descriptor> mate(V);

  // find the maximum cardinality matching. we'll use a checked version
  // of the algorithm, which takes a little longer than the unchecked
  // version, but has the advantage that it will return "false" if the
  // matching returned is not actually a maximum cardinality matching
  // in the graph.

  // times
  struct timeval  tv1, tv2;

  //start timing
  gettimeofday(&tv1, NULL);


  edmonds_maximum_cardinality_matching(g, &mate[0]);

  gettimeofday(&tv2, NULL);
  double msec = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
  printf("%f\n",msec);
   
  
  int matchnum = 0;
  graph_traits<my_graph>::vertex_iterator vi, vi_end;
  for(boost::tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi)
    if (mate[*vi] != graph_traits<my_graph>::null_vertex() && *vi < mate[*vi]) {
        matchnum++;
    }

  
  return 0;
}  
