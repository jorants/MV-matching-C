This git repository used to contain an implementation of the **Micali Vazirani Maximum Cardinality Matching Algorithm**.

The code is currently in an early stage and although the algorithm is implemented and seems to work, it really isn't user friendly.

It may currently be used as a C library, and it is possible to make pictures of the algorithm as it progresses. There are currently no bugs or show-stoppers that we know of, though the library could still use several improvements.

A faster, optimized implementation of the algorithm appears in https://github.com/jorants/MV-Matching-V2

# Obtaining the code and compiling it

To get the latest version of the code, you go to a terminal and do:

```
$ git clone https://github.com/jorants/MV-matching-C.git
```

And then:

```
$ cd MV-matching-C
$ make
```

# Typical usage of libmv

Let's call `G` to the graph you are interested in knowing the maximum cardinality matching of.
Begin by including the `libmv.h` header:

```
#include "libmv.h"
```

Now declare some `unsigned int` variable:

```
unsigned int size;
```

And do some stuff until `size` contains the number of vertices in G

Then initialize a graph structure:

```
Graph *graph = Graph_init(size)
```

Now, make sure you assign a number from `0` to `size-1` to each vertex in G. Maybe you already have this, but maybe 
your graph vertices are named some other way. In any case, enumerate them from `0` to `size-1`.

Then do some loop or something over your graph data, and for each edge (v1, v2) in G, call the function:

```
   Graph_add_edge(graph, v1, v2);
```

(Of course, `v1`, `v2` are the numbers identifying the vertices in the edge.)

When you've done that for all edges, call:

```
EdgeList *lst = MV_MaximumCardinalityMatching(graph);
```

At this point, lst is a list of edges containing a maximum cardinality matching.
To go through the individual edges in this list, use an iterator, as follows:

```
EdgeListIterator *itr = lst->first;
while (itr != 0) {
	Edge edge = itr->value;
	
	... do stuff with edge, maybe store it in your own data structure or whatever ...
	
	itr = itr->next;
}
```
To clear up the memory that was allocated, call:

```
EdgeList_delete(lst);
Graph_delete(graph);
```

And that's it.
