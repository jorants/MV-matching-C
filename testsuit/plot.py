import matplotlib
import pylab



data = map(lambda x: map(int,x.split(" ")),open("data","r").read().split("\n"))

x = map(lambda i: i[0],data)
y = map(lambda i: i[4],data)

nx = range(max(x)+1)
ny = [[] for i in range(max(x)+1)]

for i in range(len(x)):
  ny[x[i]] += [y[i]]

ny = map(lambda y: sum(y)/len(y) if len(y)!=0 else 0, ny)
matplotlib.pyplot.scatter(nx,ny)

matplotlib.pyplot.show()
