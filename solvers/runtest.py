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


def gentrig(V):
    v = 3
    Edone = []
    ETodo = [(0,1),(0,2),(1,2)]
    
    while v<V:
        edge = random.choice(ETodo)
        v1,v2 = edge
        ETodo.remove(edge)
        Edone += [edge]
        ETodo += [(v,v1),(v,v2)]
        v+=1
    g = nx.Graph()
    g.add_nodes_from(range(V))
    g.add_edges_from(Edone+ETodo)
    return g


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
    res = pross.communicate()[0].strip()
    return float(res)


def runMultiple(v,times = 10,graph_meth = lambda v: gengraph_random(v,0.4*v*(v-1)*0.5)):
    boost_speed=  0
    libmv_speed = 0
    for i in range(times):
        g = graph_meth(v)
        boost_speed+= solve_boost(g)
        libmv_speed+= solve_libmv(g)
    return (boost_speed/times,libmv_speed/times)
        
    

# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph_random(v,e):
  p = min(1,e/float(v*(v-1)/2))
  g = nx.gnp_random_graph(v,p)
  return g



def runrandoms():
    xvals = list(range(10,50,1)) + list(range(50,100,5)) + list(range(100,200,10)) + list(range(200,1000,100)) + list(range(1000,10000,500))
    yvalsboost = []
    yvalslibmv = []
    for x in xvals:
        print x
        res = runMultiple(x,10,lambda v: gengraph_random(v,0.4*(v*(v-1)*0.5)))
        yvalsboost += [res[0]]
        yvalslibmv += [res[1]]
        
    fp = open("output","w")
    fp.write(str((xvals,yvalsboost,yvalslibmv)))

def runtrigs():
    xvals = list(range(50000,50001,1))
    yvalsboost = []
    yvalslibmv = []
    for x in xvals:
        print x
        res = runMultiple(x,1,gentrig)
        yvalsboost += [res[0]]
        yvalslibmv += [res[1]]
        
    fp = open("outputtrig","w")
    fp.write(str((xvals,yvalsboost,yvalslibmv)))

    
def plot():
    import matplotlib.pyplot as plt
    data  = eval(open("output").read())
    def interpol(datax,datay,xval):
        for i,x in enumerate(datax):
            if x>xval:
                dx = float(x - datax[i-1] )
                fac1 = (xval-datax[i-1])/dx
                fac2 = (x-xval)/dx
                return datay[i-1]*fac1 + datay[i]*fac2


    xmin = data[0][0]
    xmax = data[0][-1]

    avrboost = 0
    avrlibmv = 0
    total = 0
    for i,x in enumerate(data[0]):

        if x*2 > xmax:
            break
        avrboost += interpol(data[0],data[1],2*x) / data[1][i]
        avrlibmv += interpol(data[0],data[2],2*x) / data[2][i]
        total += 1

    import math
    power_boost =  math.log(avrboost/total,2)
    power_libmv =  math.log(avrlibmv/total,2)

    print "boost complex: O(V^%f)",power_boost
    print "libmv complex: O(V^%f)",power_libmv 

    #find constant
    avrboost = 0
    avrlibmv = 0
    total = 0
    for i,x in enumerate(data[0]):
        if x<100:
            continue
        avrboost += data[1][i] / (x**power_boost)
        avrlibmv += data[2][i] / (x**power_libmv)
        total += 1
    c_boost =   avrboost/total
    c_libmv =   avrlibmv/total


    plt.plot(data[0],data[1])
    plt.plot(data[0],data[2])
    plt.plot(data[0],[ c_boost * x**power_boost for x in data[0]])
    plt.plot(data[0],[c_libmv* x**power_libmv for x in data[0]])
    plt.show()
