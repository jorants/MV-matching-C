/**
 @file
*/
#ifndef MVINFO_H
#define MVINFO_H

#include "basictypes.h"
#include "DDFSInfo.h"
#include "NodeList.h"
#include "EdgeList.h"
#include "Petal.h"
#include "Graph.h"

#define UNMATCHED INFINITY

/// this struct stores the information we will need
/// for each vertex.
typedef struct
{

  DDFSColor ddfs_color;		// dfs status

  uint evenlevel, oddlevel;

  NodeList *predecessors;	// there may be more than one predecessor

  uint ddfs_predecessor, ddfs_entry;

  //there's no list of neighbors inside a vertex. This is kept external

  Petal *petal;

  uint matched;			// which vertex it is matched with

  bool deleted;			// whether vertex has already participated in an augmenting path

 
  // list of nodes with a marked bridges towards this node,
  // whose tenacity can not be determined yet because this node
  // still didn't get its maxlevel
  NodeList *hanging_bridges;
} VertexInfo;

VertexInfo *VertexInfo_init ();	// constructor
void VertexInfo_delete (VertexInfo *);	// deconstructor

void VertexInfo_reset (VertexInfo *);	// Resets vertex info for next stage

bool VertexInfo_has_min_level (VertexInfo *);	// minlevel < infinity?
uint VertexInfo_min_level (VertexInfo *);	// min(oddlevel, evenlevel)
void VertexInfo_set_tenacity (VertexInfo * vi, uint tenacity);


typedef struct
{
  Graph *graph;
  VertexInfo **v_info;
  bool pathc;

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
  bool output;
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



#endif // MVINFO_H
