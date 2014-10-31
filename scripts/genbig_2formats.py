#!/usr/bin/env python
#
# Smal script that creates a lon of random graphs is dirac format
# edit lest 5 lines to change what to generate

import sys
import random
from random import *
import subprocess
import os

# creates a graph mith v vertexices and e edges and adds index i to name
def gengraph(v):
    fp = open("big.txt","w")
    fp2 = open("big2.txt","w")

    fp.write("p edge %i %i\n" %(v,0))
    fp.write("%i %i\n" %(v,0))
    
    count = 0

    E = []
    for a in range(v):
        for b in range(a+1,v):
            if a==b:
                continue
            if randint(0,1)== 1:
                
                
                fp.write("e %i %i 1\n" %(a+1,b+1))
                count +=1
    fp.close()
    print(count)



gengraph(5001)
