import sys
import random


if len(sys.argv) >= 4:
  matchedn  =  2*int(sys.argv[1])
  unmatchedn  =  int(sys.argv[2])
  addedge  =  int(sys.argv[3])
else:
  print("Usage: %s {matching number}  {number of unmatched vertices} {number of extra edges} [filename]")
  sys.exit(0)


n = matchedn + unmatchedn
if (n-1)* n /2 - n/2 < addedge:
  print("can't add that many edges")
  sys.exit(0)


if len(sys.argv) >= 5:
  fp = open(sys.argv[4],"w")
else:
  fp = sys.stdout


points = list(range(n)) 

matchedv = list(range(matchedn)) 
unmatchedv = list(range(matchedn,matchedn+unmatchedn)) 
matchede = [[i,i+1] for i in range(0,matchedn,2)]
unmatchede = []


def lists_overlap3(a, b):
    return bool(set(a) & set(b))

def hasAugmening(matchedv,unmatchedv,matchede,unmatchede):
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
    if hasAugmening(matchedv,unmatchedv,matchede,new_unmatchede):
      continue
    unmatchede = new_unmatchede
    break
  if tries == 1000:
    sys.stderr.write("failed")
    sys.close()

fp.write("%i\n\n" %(n))
for x in matchede:
  fp.write("%i -- %i\n" %(x[0],x[1]))

for x in unmatchede:
  fp.write("%i - %i\n" %(x[0],x[1]))



