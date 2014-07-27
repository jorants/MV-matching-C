import sys
import random
from random import shuffle
import subprocess

def writeGraph(V,E, fn):
    fp = open(fn,"w")
    fp.write("p edge %i %i\n" %(len(V),len(E)))
    for x in E:
      fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))
    fp.close()

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


for v in range(20):
  print(v,int(v*(v-1)/2))
  for e in range(int(v*(v-1)/2)):
     for i in range(10):
        gengraph(v,e,i)

