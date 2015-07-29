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
import subprocess

def WriteGraphData(V,E,fp):
    fp.write("p edge %i %i\n" %(V,len(E)))
    for x in E:
        fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))


def solve_boost(g):
    pross = subprocess.Popen(["./solver-boost"],stdout = subprocess.PIPE,stdin = subprocess.PIPE)
    WriteGraphData(g.number_of_nodes(),g.edges(),pross.stdin)
    return float(pross.communicate()[0].strip())

def solve_libmv(g):
    pross = subprocess.Popen(["./solver-libmv"],stdout = subprocess.PIPE,stdin = subprocess.PIPE)
    WriteGraphData(g.number_of_nodes(),g.edges(),pross.stdin)
    return float(pross.communicate()[0].strip())


def runMultiple(v,p = 0.4,times = 10):
    boost_speed=  0
    libmv_speed = 0
    for i in range(times):
        g = gengraph(v,p*v*(v-1)*0.5)
        boost_speed+= solve_boost(g)
        libmv_speed+= solve_libmv(g)
    return (boost_speed/times,libmv_speed/times)
        
    
# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph(v,e):
  p = e/float(v*(v-1)/2)
  g = nx.gnp_random_graph(v,p)
  return g



xvals = list(range(10,50,1)) + list(range(50,100,5)) + list(range(100,200,10)) + list(range(200,1000,100))
yvalsboost = []
yvalslibmv = []
for x in xvals:
    print x
    res = runMultiple(x)
    yvalsboost += [res[0]]
    yvalslibmv += [res[1]]

fp = open("output","w")
fp.write(str((xvals,yvalsboost,yvalslibmv)))

