
import sys

fp = open(sys.argv[1] ,"r") if len(sys.argv) >1 else sys.stdin
data = fp.read()

fpout = open(sys.argv[2] ,"w") if len(sys.argv) >2 else sys.stdout

data = data.split("\n")

V = int(data[0])
data = data[2:]
E = []
for x in data:
  line = list(filter(len,x.split(" ")))
  if len(line)>2:
    E+=[(int(line[0]),int(line[2]))]


fpout.write("p edge %i %i\n" % (V,len(E)))
for e in E:
  fpout.write("e %s %s 1\n" % (e[0]+1,e[1]+1))

fpout.close()
