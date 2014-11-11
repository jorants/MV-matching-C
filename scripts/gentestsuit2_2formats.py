#!/usr/bin/env python
#
# Smal script that creates a lot of random graphs is dirac format
# edit lest 5 lines to change what to generate

import sys
import random
from random import shuffle
import subprocess
import os
import networkx as nx

def writeGraph(V,E, fn):
    fp = open(fn,"w")
    fp.write("p edge %i %i\n" %(V,len(E)))
    for x in E:
      fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))
    fp.close()

def writeGraph2(V,E, fn):
    fp = open(fn,"w")
    fp.write("%i %i U\n" %(V,len(E)))
    
    for node in range(V):
      edges = filter(lambda x: node in x, E)
      fp.write("%i 0 0 0" % (len(edges)))
      for edge in edges:
	fp.write("%i 0\n" %(edge[0] if edge[0] != node else edge[1]))
    fp.close()

# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph(v,e,k):
  p = e/float(v*(v-1)/2)
  g = nx.gnp_random_graph(v,p)
  writeGraph(v,g.edges(), "out/%i_%i_%i.txt" %(v,e,k))
  # writeGraph2(v,g.edges(), "out2/%i_%i_%i.txt" %(v,e,k))
  return True


for v in range(10,60):
   print(v)
   for e in range(int(v*(v-1)/16),int(v*(v-1)/8), 1 ):
      for i in range(2):
         gengraph(v,e,i)

# gengraph(200,10000,1)