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
  p = e/float(v*(v-1)/2)
  g = nx.gnp_random_graph(v,p)
  writeGraph(v,g.edges(), "out/%i_%i_%i.txt" %(v,e,k))
  return True


for v in range(10,60):
   print(v)
   for e in range(1,int(v*(v-1)/2), int(v*(v-1)/200) + 1 ):
      for i in range(2):
         gengraph(v,e,i)
