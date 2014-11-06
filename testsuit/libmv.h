typedef unsigned int uint;


typedef struct
{
  uint v1, v2;
} Edge;

_Bool ansi_c_getline (FILE * fp, char **line_out, uint * length_out);
_Bool str_starts_with (char *str, char *prefix);
int file_exists (const char *fname);

_Bool b_print_mv_progress;
/**
 @file
*/



/**
 @file
 */
/**
 @file
*/



/**
   @file
*/



/**
 @file
 */




typedef struct NodeListIterator
{
  uint value;
  struct NodeListIterator *next;
} NodeListIterator;

/**
   typical use
   NodeList *lst = NodeList_init();
   ..
   NodeList_add(lst, element)
   ...
   NodeListIterator *itr = lst->first;
   while (itr != 0) {
   uint elm = itr->value;

   ... do stuff with elm...

   itr = itr->next;
   }
*/
typedef struct
{
  NodeListIterator *first, *last;
} NodeList;

NodeList *NodeList_init ();
void NodeList_delete (NodeList * lst);

_Bool NodeList_is_empty (NodeList * lst);
_Bool NodeList_is_empty_or_singleton (NodeList * lst);

void NodeList_add (NodeList * lst, uint elm);
void NodeList_push (NodeList * lst, uint elm);

uint NodeList_pop (NodeList * lst); // removes first element and returns it
uint NodeList_top (NodeList * lst); // returns first element
uint NodeList_nth (NodeList * lst, uint n); // returns nth element (first = 0th)


void NodeList_set_top (NodeList * lst, uint newtop);

void NodeList_erase (NodeList * lst); // empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void NodeList_append (NodeList * to, NodeList * from);
void NodeList_append_reverse (NodeList * to, NodeList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void NodeList_join (NodeList * to, NodeList * from);


_Bool NodeList_find (NodeList * lst, uint elm);
/**
  @file
*/




/**
 @file
 */




typedef struct PointerListIterator
{
  void* value;
  struct PointerListIterator *next;
} PointerListIterator;


/**
 typical use
PointerList *lst = PointerList_init();
..
PointerList_add(lst, element)
...
PointerListIterator *itr = lst->first;
while (itr != 0) {
	uint elm = itr->value;
	
	... do stuff with elm...

	itr = itr->next;
}
*/
typedef struct
{
  PointerListIterator *first, *last;
} PointerList;

PointerList *PointerList_init ();
void PointerList_delete (PointerList * lst);

_Bool PointerList_is_empty (PointerList * lst);
_Bool PointerList_is_empty_or_singleton (PointerList * lst);

void PointerList_add (PointerList * lst, void* elm);
void PointerList_push (PointerList * lst, void* elm);

void* PointerList_pop (PointerList * lst); // removes first element and returns it
void* PointerList_top (PointerList * lst); // returns first element
void* PointerList_nth (PointerList * lst, uint n); // returns nth element (first = 0th)


void PointerList_set_top (PointerList * lst, void* newtop);

void PointerList_erase (PointerList * lst); // empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void PointerList_append (PointerList * to, PointerList * from);
void PointerList_append_reverse (PointerList * to, PointerList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void PointerList_join (PointerList * to, PointerList * from);


_Bool PointerList_find (PointerList * lst, void* elm);
/**
 @file
*/





/**
 @file
 */
/**
   @file
*/
/**
  @file
*/


typedef struct
{
  uint bluenode, rednode, base;
  uint link;
  uint* linkholder;
  NodeList *support;
} Petal;


/// creates a new petal, indexes it in the given pointer list
Petal *Petal_init (PointerList * ptrs);
/// Deletes all petals indexed in the pointer list
/// does NOT delete the pointerlist itself,
/// and the pointers to the deleted petals become invalid
void Petal_deleteAll (PointerList * ptrs);

// DDFS Colors
typedef enum
{ none = 0, red, blue } DDFSColor;

typedef struct
{
  PointerList *bStack, *rStack;
  // blue and red unexausted nodes in stack
  uint bNumUnexausted, rNumUnexausted;
  Petal *petal;

} DDFSInfo;

DDFSInfo *DDFSInfo_init (PointerList * petals);

void DDFSInfo_delete (DDFSInfo *);

void DDFSInfo_push (DDFSInfo *, DDFSColor, NodeListIterator *);
uint DDFSInfo_top (DDFSInfo *, DDFSColor);
/**
   @file
*/
/**
 @file
*/



/**
 @file
 */




typedef struct EdgeListIterator
{
  Edge value;
  struct EdgeListIterator *next;
} EdgeListIterator;


/**
 typical use
EdgeList *lst = EdgeList_init();
..
EdgeList_add(lst, element)
...
EdgeListIterator *itr = lst->first;
while (itr != 0) {
	uint elm = itr->value;
	
	... do stuff with elm...

	itr = itr->next;
}
*/
typedef struct
{
  EdgeListIterator *first, *last;
} EdgeList;

EdgeList *EdgeList_init ();
void EdgeList_delete (EdgeList * lst);

_Bool EdgeList_is_empty (EdgeList * lst);
_Bool EdgeList_is_empty_or_singleton (EdgeList * lst);

void EdgeList_add (EdgeList * lst, Edge elm);
void EdgeList_push (EdgeList * lst, Edge elm);

Edge EdgeList_pop (EdgeList * lst); // removes first element and returns it
Edge EdgeList_top (EdgeList * lst); // returns first element
Edge EdgeList_nth (EdgeList * lst, uint n); // returns nth element (first = 0th)


void EdgeList_set_top (EdgeList * lst, Edge newtop);

void EdgeList_erase (EdgeList * lst); // empties the list

/**
 * Copies the contents of list \a from to \a to.
 * @param to List where the contents are copied to.
 * @param from List where contents are copied from.
 */
void EdgeList_append (EdgeList * to, EdgeList * from);
void EdgeList_append_reverse (EdgeList * to, EdgeList * from);

/**
 * Joins the list \a from to the list \a to. Does not copy the data,
 * but merely sets the next pointer of the last element of the list \a to
 * to the first element of the list \a from.
 *
 * It then deletes the from list, so there are no conflicts.
 * @param to The list to which the other list is joined to.
 * @param from The list which is joined.
 */
void EdgeList_join (EdgeList * to, EdgeList * from);


_Bool EdgeList_find (EdgeList * lst, Edge elm);
/**
 @file
*/






/**
 @file
 */
/**
   @file
*/


/**
 * Basic graph
 */
typedef struct
{
  //! number of nodes
  uint size;
  //! All the edges
  NodeList **edges;
} Graph;


Graph *Graph_init (uint size);
void Graph_delete (Graph * g);

void Graph_add_edge (Graph * g, uint v1, uint v2);
NodeListIterator *Graph_neighbours (Graph * g, uint v);

Graph *Graph_init_file (char *filename);
Graph *Graph_init_triangles (uint k);
Graph *Graph_init_triangles2 (uint k);
Graph *Graph_init_hardcard (uint k);


Graph *Graph_init_random (uint size, uint modulus);



/// this struct stores the information we will need
/// for each vertex.
typedef struct
{

  DDFSColor ddfs_color; // dfs status

  uint evenlevel, oddlevel;

  NodeList *predecessors; // there may be more than one predecessor

  uint ddfs_predecessor, ddfs_entry;

  //there's no list of neighbors inside a vertex. This is kept external

  Petal *petal;

  uint matched; // which vertex it is matched with

  _Bool deleted; // whether vertex has already participated in an augmenting path


  // list of nodes with a marked bridges towards this node,
  // whose tenacity can not be determined yet because this node
  // still didn't get its maxlevel
  NodeList *hanging_bridges;
} VertexInfo;

VertexInfo *VertexInfo_init (); // constructor
void VertexInfo_delete (VertexInfo *); // deconstructor

void VertexInfo_reset (VertexInfo *); // Resets vertex info for next stage

_Bool VertexInfo_has_min_level (VertexInfo *); // minlevel < infinity?
uint VertexInfo_min_level (VertexInfo *); // min(oddlevel, evenlevel)
void VertexInfo_set_tenacity (VertexInfo * vi, uint tenacity);


typedef struct
{
  Graph *graph;
  VertexInfo **v_info;
  _Bool pathc;

  int * links; // array of links

  /// stores the nodes of different levels;
  /// a single node will be in at most two such levels (one even, one odd).
  NodeList **levels;

  /// list of bridges for different tenacities
  /// bridges in bridge[i] have tenacity 2 i + 1
  EdgeList **bridges;
  ///
  /// list of petals
  PointerList *petals;
  ///
  /// current stage and level of the algorithm
  uint stage, level;

  uint matched_num;
  EdgeList *redEdges, *blueEdges;
  Edge currentDDFSBridge;
  EdgeList *aug_path;
  ///
  /// Defines whether to output dot files or not
  _Bool output;
  ///
  /// folder to write to if output is enabled
  char *outpath;
} MVInfo;

MVInfo *MVInfo_init (Graph * g);
void MVInfo_delete (MVInfo * mvi);
void MVInfo_next_stage (MVInfo * mvi);
void MVInfo_next_level (MVInfo * mvi);


void MVInfo_print_graphviz (MVInfo * mvi, char *filename);
void MVInfo_print_contents (MVInfo * mvi, char *filename);

/**
  Main way to start using the algorithem, parses an input file to the internal format.
  NOT A SAFE FUNCTION: only pass correct input.
  @param filename path to a simple-syntax graph file
  @returns A pointer to a MVInfo object, ready to be passed to the Maximum Cardinalaty fonctions
*/
MVInfo *MVInfo_init_file_simple (char *filename);

/**
  Main way to start using the algorithem, parses an input file to the internal format.
  NOT A SAFE FUNCTION: only pass correct input.
  @param filename path to a DIRAC syntax graph file
  @returns A pointer to a MVInfo object, ready to be passed to the Maximum Cardinalaty fonctions
*/
MVInfo *MVInfo_init_file (char *filename);
/**
 * Goes through every edge in the current level nodes,
 * assigns new minlevels and discovers next level nodes,
 * attributes predecessors, and marks edges as bridges
 * @param mvi Basic algoritmic information
 */
void MV_BFS_step (MVInfo * mvi);

/**
 * does a DDFS to find an augmenting path by  looking from both sides of a bridge. 
 * @return true if augmenting path was found
 * @param mvi Basic algoritmic information
 * @param bridge The bridge found in the previous stage from which to look for a augmenting path
 */
_Bool MV_DDFS (MVInfo * mvi, Edge bridge);



void MV_Augment (MVInfo * mvi, DDFSInfo * path);

EdgeList *MV_MaximumCardinalityMatching (Graph * g);
EdgeList *MV_MaximumCardinalityMatching_ (MVInfo * mvi);
