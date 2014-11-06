import sys

import numpy

top  = 1000
data = map(lambda x: map(int,x.split(" ")),open(sys.argv[1],"r").read().strip().split("\n"))

x = map(lambda i: i[0],data)
y = map(lambda i: i[4],data)
y2 = map(lambda i: i[5],data)
y6 = map(lambda i: i[9],data)

nx = range(max(x)+1)
ny = [[] for i in range(max(x)+1)]

ny2 = [[] for i in range(max(x)+1)]
ny6 = [[] for i in range(max(x)+1)]

for i in range(len(x)):
  tx = x[i]
  if tx<top:
    ny[tx] += [y[i]]
    ny2[tx] += [y2[i]]
    ny6[tx] += [y6[i]]


ny = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny)
ny2 = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny2)
ny6= map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny6)

print ny
