#!/usr/bin/env python
#
# Smal script that creates a lon of random graphs is dirac format
# edit lest 5 lines to change what to generate

import sys
import random
from random import shuffle
import subprocess
import os

def writeGraph(V,E, fn):
    fp = open(fn,"w")
    fp.write("p edge %i %i\n" %(len(V),len(E)))
    for x in E:
      fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))
    fp.close()

# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph(v,e,k):
  V = range(v)
  E = []
  for i in range(e):
    tries = 0
    while True:
      tries +=1
      if tries > 1000:
        return False
      edge = list(sorted(random.sample(V, 2)))
      if edge in E:
        continue #already an edge
      else:
         E+=[edge]
         break
  writeGraph(V,E, "out/%i_%i_%i.txt" %(v,e,k))
  return True


for v in range(500,1000):
   print(v)
   #for e in range(1,int(v*(v-1)/2) ):
   e = 5*v
   for i in range(10):
        gengraph(v,e,i)
