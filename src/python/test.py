import networkx
import mv
g = networkx.tutte_graph()
print mv.matching_from_networkx(g)
