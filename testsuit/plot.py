import matplotlib
import pylab



data = map(lambda x: map(int,x.split(" ")),open("data","r").read().strip().split("\n"))

x = map(lambda i: i[0],data)
y = map(lambda i: i[4],data)
y2 = map(lambda i: i[5],data)
y3 = map(lambda i: i[6],data)
y4 = map(lambda i: i[6],data)

nx = range(max(x)+1)
ny = [[] for i in range(max(x)+1)]
ny2 = [[] for i in range(max(x)+1)]
ny3 = [[] for i in range(max(x)+1)]
ny4 = [[] for i in range(max(x)+1)]

for i in range(len(x)):
  tx = x[i]
  ny[tx] += [y[i]]
  ny2[tx] += [y2[i]]
  if y3[i]!= -1:
    ny3[tx] += [y4[i]]
  if y4[i]!= -1:
    ny4[tx] += [y4[i]]

ny = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny)
ny2 = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny2)
ny3 = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny3)
ny4= map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny4)

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

#matplotlib.pyplot.scatter(nx,ny)
matplotlib.pyplot.scatter(nx,ny2,color="red")
#matplotlib.pyplot.scatter(nx,ny3,color="green")
matplotlib.pyplot.scatter(nx,ny4,color="black")

matplotlib.pyplot.show()
