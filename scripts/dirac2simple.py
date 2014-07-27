
import sys

fp = open(sys.argv[1] ,"r") if len(sys.argv) >1 else sys.stdin
data = fp.read()

fpout = open(sys.argv[2] ,"w") if len(sys.argv) >2 else sys.stdout

data = data.split("\n")
data = list(map(lambda x : x.split(" "),data))

V = int(data[0][2])

data = data[1:]
E = []
for x in data:
  if len(x)>2:
    E+=[(int(x[1])-1,int(x[2])-1)]


fpout.write("%i\n\n" % (V))
for e in E:
  fpout.write("%s - %s\n" % (e[0],e[1]))

fpout.close()
