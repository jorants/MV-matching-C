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
i = 0
for i,x in enumerate(data[0]):

    if x*2 > xmax:
        break
    avrboost += interpol(data[0],data[1],2*x) / data[1][i]
    avrlibmv += interpol(data[0],data[2],2*x) / data[2][i]
    i += 1

power_boost =  avrboost/i
power_libmv =  avrlibmv/i


#find constant
avrboost = 0
avrlibmv = 0
i = 0
for i,x in enumerate(data[0]):
    if x<100:
        continue
    avrboost += data[1][i] / (x**power_boost)
    avrlibmv += data[2][i] / (x**power_libmv)
    i += 1
c_boost =   avrboost/i
c_libmv =   avrlibmv/i

print c_boost
print c_libmv
print data[1][-1]

plt.plot(data[0],data[1])
plt.plot(data[0],data[2])
plt.plot(data[0],[(data[1][-1]/(xmax**power_boost))* x**power_boost for x in data[0]])
plt.plot(data[0],[(data[2][-1]/(xmax**power_libmv))* x**power_libmv for x in data[0]])
plt.show()
