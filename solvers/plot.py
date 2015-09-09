import matplotlib.pyplot as plt

data  = eval(open("outputtrig").read())


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


plt.plot(data[0],data[1],color= "red") #boost
plt.plot(data[0],data[2],color = "blue") #mv maching
#plt.plot(data[0],[ c_boost * x**power_boost for x in data[0]])
#plt.plot(data[0],[c_libmv* x**power_libmv for x in data[0]])
plt.show()
