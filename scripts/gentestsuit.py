import sys
import random
from random import shuffle
import subprocess

def gengraph(matchedn,unmatchedn, addedge):
  matchedn *= 2
  n = matchedn + unmatchedn
  if (n-1)* n /2 - n/2 < addedge:
    return False



  points = list(range(n)) 
  matchedv = list(range(matchedn)) 
  unmatchedv = list(range(matchedn,matchedn+unmatchedn)) 
  matchede = [[i,i+1] for i in range(0,matchedn,2)]
  unmatchede = []


  def lists_overlap3(a, b):
      return bool(set(a) & set(b))

  #works only most of the time for some reason, need a double chek
  def hasAugmenting(matchedv,unmatchedv,matchede,unmatchede):
    #BFS for each unmatched vertex to trie and walk to an other unmatched one
    for topnode in unmatchedv:
      matchedtogo = set(matchedv) #points not visited yet
      nowpoints = set([topnode]) #ponts in this depth of the tree
      stage = 1 #next should be unmatched edge, -1 for matched

      while len(nowpoints) != 0: #while there is stil work to do
        nextpoints = set([])
        for p in nowpoints:
          if stage == 1: #only walk over unmatched edges
            walkable = filter(lambda edge: p in edge, unmatchede)
          else:          #only walk over matched edges
            walkable = filter(lambda edge: p in edge, matchede)

          #make edges into neigbours
          neigbours = set(map(lambda edge: edge[1] if edge[0] == p else edge[0],walkable))

          if stage == 1 and bool(set(unmatchedv) & neigbours):
             #we can walk to an unmatched vertex! this cant be the topnode so there is a path.
             return True 
          #only visit nodes once, no loops
          neigbours = set(filter(lambda buur: buur in matchedtogo,neigbours))
          nextpoints.update(neigbours)
          #remove them from points to visit
          matchedtogo.difference_update(neigbours)
        stage *= -1
        nowpoints = nextpoints
    return False


  def writeGraph(matchedv,unmatchedv,matchede,unmatchede, fn):
    fp = open(fn,"w")
    fp.write("p edge %i %i\n" %(len(matchedv)+len(unmatchedv),len(matchede)+len(unmatchede)))
    for x in matchede:
      fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))
    for x in unmatchede:
      fp.write("e %i %i 1\n" %(x[0]+1,x[1]+1))

  #gets matchnum from solver 3
  def getMatchnum(matchedv,unmatchedv,matchede,unmatchede):
    writeGraph(matchedv,unmatchedv,matchede,unmatchede, "tmp.txt")
    out = str(subprocess.Popen(["./match < tmp.txt"], shell=True ,stdout=subprocess.PIPE).communicate()[0])
    out = out.split("\n")
    f = out[0].split(" ")
    return int(f[-1]) if f[-1].isdigit() else -1

  for i in range(addedge):
    tries = 0
    while tries < 1000:
      tries +=1
      edge = list(sorted(random.sample(points, 2)))
      if edge in matchede or edge in unmatchede:
        continue #already an edge
      if edge[0] in unmatchedv and edge[1] in unmatchedv:
        continue #would increase matching number
      new_unmatchede = unmatchede+[edge]
      if hasAugmenting(matchedv,unmatchedv,matchede,new_unmatchede):
        continue
      unmatchede = new_unmatchede
      break
    if tries == 1000:
      return False
  mn = getMatchnum(matchedv,unmatchedv,matchede,unmatchede)
  if mn != matchedn/2:
       print("want: %i   but got %i" %(matchedn/2,mn))
       return True
  nodes = list(range(n))
  shuffle(nodes) 
  matchede = list(map(lambda x: [nodes[x[0]],nodes[x[1]]],matchede))
  unmatchede = list(map(lambda x: [nodes[x[0]],nodes[x[1]]],unmatchede))
  writeGraph(matchedv,unmatchedv,matchede,unmatchede, "out/%i_%i_%i.txt" %(matchedn/2,unmatchedn,addedge) )
  return True



for m in range(20):
 print(m)
 for n in range(30):
  for a in range(400):
   if not gengraph(m,n,a):
     break

