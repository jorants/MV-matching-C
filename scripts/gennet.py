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

# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph(v,e,k):
  g = nx.barabasi_albert_graph(v,e)  
  writeGraph(v,g.edges(), "out/%i_%i_%i.txt" %(v,e,k))
  return True


for v in range(10,5000):
   print(v)
   for e in range(4, 5):
      for i in range(10):
         gengraph(v,e,i)
