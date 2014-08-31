import matplotlib
import pylab
import sys

import numpy

top  = 1000
data = map(lambda x: map(int,x.split(" ")),open(sys.argv[1],"r").read().strip().split("\n"))

x = map(lambda i: i[0],data)
y = map(lambda i: i[4],data)
y2 = map(lambda i: i[5],data)
y3 = map(lambda i: i[6],data)
y4 = map(lambda i: i[7],data)
y5 = map(lambda i: i[8],data)
y6 = map(lambda i: i[9],data)

nx = range(max(x)+1)
ny = [[] for i in range(max(x)+1)]

ny2 = [[] for i in range(max(x)+1)]

ny3 = [[] for i in range(max(x)+1)]
ny4 = [[] for i in range(max(x)+1)]
ny5 = [[] for i in range(max(x)+1)]
ny6 = [[] for i in range(max(x)+1)]

for i in range(len(x)):
  tx = x[i]
  if tx<top:
   if tx%2 == 0:
    ny[tx] += [y[i]]
    ny2[tx] += [y2[i]]
   else:
    ny[tx] += [y[i]]
    ny2[tx] += [y2[i]]

   if y3[i]!= -1:
     ny3[tx] += [y3[i]]
   if y4[i]!= -1:
     ny4[tx] += [y4[i]]
   if y5[i]!= -1:
     ny5[tx] += [y5[i]]
   if y6[i]!= -1:
     ny6[tx] += [y6[i]]


ny = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny)
ny2 = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny2)

ny3 = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny3)
ny4= map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny4)
ny5= map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny5)
ny6= map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny6)


minimum  = 10000000 
for x in ny:
  if x>0 and x< minimum:
     minimum = x
ny = list(map(lambda x: max(x-minimum,0),ny))


minimum  = 10000000 
for x in ny2:
  if x>0 and x< minimum:
     minimum = x
ny2 = list(map(lambda x: max(x-minimum,0),ny2))


minimum  = 10000000 
for x in ny3:
  if x>0 and x< minimum:
     minimum = x
ny3 = list(map(lambda x: max(x-minimum,0),ny3))

minimum  = 10000000 
for x in ny4:
  if x>0 and x< minimum:
     minimum = x
ny4 = list(map(lambda x: max(x-minimum,0),ny4))
minimum  = 10000000 
for x in ny5:
  if x>0 and x< minimum:
     minimum = x
ny5 = list(map(lambda x: max(x-minimum,0),ny5))

minimum  = 10000000 
for x in ny6:
  if x>0 and x< minimum:
     minimum = x
ny6 = list(map(lambda x: max(x-minimum,0),ny6))


matplotlib.pyplot.scatter(nx[:top],ny[:top],color="blue",marker=".")
matplotlib.pyplot.scatter(nx[:top],ny2[:top],color="red",marker=".")
matplotlib.pyplot.scatter(nx[:top],ny3[:top],color="green",marker=".")
matplotlib.pyplot.scatter(nx[:top],ny4[:top],color="black",marker=".")
matplotlib.pyplot.scatter(nx[:top],ny5[:top],color="pink",marker=".")
matplotlib.pyplot.scatter(nx[:top],ny6[:top],color="orange",marker=".")
matplotlib.pyplot.savefig(sys.argv[2])
sys.exit(0)

def makelinlist(x,a,b):
  return [a*k+b for k in x]

res = numpy.polyfit(nx[0:top:2], nya[0:top:2], 1)

matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="blue",marker=".")

res = numpy.polyfit(nx[0:top:2], ny2a[0:top:2], 1)

matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="red",marker=".")


res = numpy.polyfit(nx[1:top:2], nyb[1:top:2], 1)

matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="blue",marker=".")

res = numpy.polyfit(nx[1:top:2], ny2b[1:top:2], 1)

matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="red",marker=".")


res = numpy.polyfit(nx[:top], ny3[:top], 1)
matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="green",marker=".")
res = numpy.polyfit(nx[:top], ny4[:top], 1)
matplotlib.pyplot.scatter(nx[:top],makelinlist(nx[:top],res[0],res[1]),color="black",marker=".")


matplotlib.pyplot.savefig(sys.argv[2])
