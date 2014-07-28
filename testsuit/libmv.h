/** @file
 * Header for the libairy
 */
#ifndef MVLIB_H
#define MVLIB_H


typedef unsigned int uint;


typedef struct {
 uint v1, v2;
} Edge;

_Bool ansi_c_getline(FILE *fp, char **line_out, uint *length_out);
_Bool str_starts_with(char *str, char *prefix);
int file_exists(const char *fname);

_Bool b_print_mv_progress;

typedef struct NodeListIterator
{
    uint value;
    struct NodeListIterator *next;
} NodeListIterator;

typedef struct
{
    NodeListIterator *first, *last;
} NodeList;

NodeList *NodeList_init();
void NodeList_delete(NodeList *lst);

_Bool NodeList_is_empty(NodeList *lst);
_Bool NodeList_is_empty_or_singleton(NodeList *lst);

void NodeList_add(NodeList *lst, uint elm);
void NodeList_push(NodeList *lst, uint elm);

uint NodeList_pop(NodeList *lst);
uint NodeList_top(NodeList *lst);
uint NodeList_nth(NodeList *lst, uint n);


void NodeList_set_top(NodeList *lst, uint newtop);

void NodeList_erase(NodeList *lst);






void NodeList_append(NodeList *to, NodeList *from);
void NodeList_append_reverse(NodeList *to, NodeList *from);

void NodeList_join(NodeList *to, NodeList *from);


_Bool NodeList_find(NodeList *lst, uint elm);

typedef struct PointerListIterator
{
    void* value;
    struct PointerListIterator *next;
} PointerListIterator;

typedef struct
{
    PointerListIterator *first, *last;
} PointerList;

PointerList *PointerList_init();
void PointerList_delete(PointerList *lst);

_Bool PointerList_is_empty(PointerList *lst);
_Bool PointerList_is_empty_or_singleton(PointerList *lst);

void PointerList_add(PointerList *lst, void* elm);
void PointerList_push(PointerList *lst, void* elm);

void* PointerList_pop(PointerList *lst);
void* PointerList_top(PointerList *lst);
void* PointerList_nth(PointerList *lst, uint n);


void PointerList_set_top(PointerList *lst, void* newtop);

void PointerList_erase(PointerList *lst);






void PointerList_append(PointerList *to, PointerList *from);
void PointerList_append_reverse(PointerList *to, PointerList *from);

void PointerList_join(PointerList *to, PointerList *from);


_Bool PointerList_find(PointerList *lst, void* elm);

typedef struct
{
    uint bluenode, rednode, base;

    NodeList *support;
} Petal;



Petal *Petal_init(PointerList *ptrs);



void Petal_deleteAll(PointerList *ptrs);



typedef enum {none = 0, red, blue} DDFSColor;

typedef struct
{
    PointerList *bStack, *rStack;

    uint bNumUnexausted, rNumUnexausted;
    Petal *petal;

} DDFSInfo;

DDFSInfo *DDFSInfo_init(PointerList *petals);

void DDFSInfo_delete(DDFSInfo *);

void DDFSInfo_push(DDFSInfo *, DDFSColor, NodeListIterator *);
uint DDFSInfo_top(DDFSInfo *, DDFSColor);

typedef struct EdgeListIterator
{
    Edge value;
    struct EdgeListIterator *next;
} EdgeListIterator;

typedef struct
{
    EdgeListIterator *first, *last;
} EdgeList;

EdgeList *EdgeList_init();
void EdgeList_delete(EdgeList *lst);

_Bool EdgeList_is_empty(EdgeList *lst);
_Bool EdgeList_is_empty_or_singleton(EdgeList *lst);

void EdgeList_add(EdgeList *lst, Edge elm);
void EdgeList_push(EdgeList *lst, Edge elm);

Edge EdgeList_pop(EdgeList *lst);
Edge EdgeList_top(EdgeList *lst);
Edge EdgeList_nth(EdgeList *lst, uint n);


void EdgeList_set_top(EdgeList *lst, Edge newtop);

void EdgeList_erase(EdgeList *lst);






void EdgeList_append(EdgeList *to, EdgeList *from);
void EdgeList_append_reverse(EdgeList *to, EdgeList *from);

void EdgeList_join(EdgeList *to, EdgeList *from);


_Bool EdgeList_find(EdgeList *lst, Edge elm);

typedef struct
{

    uint size;

    NodeList **edges;
} Graph;


Graph *Graph_init(uint size);
void Graph_delete(Graph *g);

void Graph_add_edge(Graph *g, uint v1, uint v2);
NodeListIterator *Graph_neighbours(Graph *g, uint v);

Graph *Graph_init_file(char *filename);
Graph *Graph_init_triangles(uint k);
Graph *Graph_init_triangles2(uint k);
Graph *Graph_init_hardcard(uint k);


Graph *Graph_init_random(uint size, uint modulus);






typedef struct
{

    DDFSColor ddfs_color;

    uint evenlevel, oddlevel;

    NodeList *predecessors;

    uint ddfs_predecessor, ddfs_entry;



    Petal *petal;

    uint matched;

    _Bool deleted;




    NodeList *hanging_bridges;
} VertexInfo;

VertexInfo *VertexInfo_init();
void VertexInfo_delete(VertexInfo *);

void VertexInfo_reset(VertexInfo *);

_Bool VertexInfo_has_min_level(VertexInfo *);
uint VertexInfo_min_level(VertexInfo *);
void VertexInfo_set_tenacity(VertexInfo *vi, uint tenacity);


typedef struct
{
    Graph *graph;
    VertexInfo **v_info;

  _Bool pathc;

    NodeList **levels;



    EdgeList **bridges;

    PointerList *petals;




    uint stage, level;


    EdgeList *redEdges, *blueEdges;
    Edge currentDDFSBridge;
    EdgeList *aug_path;

    _Bool output;

  char* outpath;
} MVInfo;

MVInfo *MVInfo_init(Graph *g);
void MVInfo_delete(MVInfo *mvi);
void MVInfo_next_stage(MVInfo *mvi);
void MVInfo_next_level(MVInfo *mvi);


void MVInfo_print_graphviz(MVInfo *mvi, char *filename);
void MVInfo_print_contents(MVInfo *mvi, char *filename);


MVInfo *MVInfo_init_file(char *filename);

void MV_BFS_step(MVInfo *mvi);

_Bool MV_DDFS(MVInfo *mvi, Edge bridge);



void MV_Augment(MVInfo *mvi, DDFSInfo *path);

EdgeList *MV_MaximumCardinalityMatching(Graph *g);
EdgeList *MV_MaximumCardinalityMatching_(MVInfo *mvi);

#endif
