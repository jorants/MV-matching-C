/** @file
 * Contains the main algoritem
 *
 */
#ifndef MV_H
#define MV_H

#include "basictypes.h"
#include "MVInfo.h"
/**
 * Goes through every edge in the current level nodes,
 * assigns new minlevels and discovers next level nodes,
 * attributes predecessors, and marks edges as bridges
 * @param mvi Basic algoritmic information
 */
void MV_BFS_step (MVInfo * mvi);

/**
 * does a DDFS to find an augmenting path by looking from both sides of a bridge. 
 * @return true if augmenting path was found
 * @param mvi Basic algoritmic information
 * @param bridge The bridge found in the previous stage from which to look for a augmenting path
 */
bool MV_DDFS (MVInfo * mvi, Edge bridge);

/**
 * Given a DDFSInfo structure for a double-depth-first-search which succeeded in finding an augmenting path,
 * changes the current matching by augmenting along that path.
 * @param mvi Basic algorithm information
 * @param path A DDFSInfo structure for a successful ddfs
 */
void MV_Augment (MVInfo * mvi, DDFSInfo * path);

/**
 * Given a graph g, returns a maximum cardinality matching for g.
 * @return An EdgeList instance; the edges in the list form a maximum-cardinality matching.
 * @param g A graph.
 */
EdgeList *MV_MaximumCardinalityMatching (Graph * g);

/**
 * Given an initialized MVInfo instance, returns a maximum cardinality matching for its graph.
 * @return An EdgeList instance; the edges in the list form a maximum-cardinality matching.
 * @param mvi The initialized MVInfo instance.
 */
EdgeList *MV_MaximumCardinalityMatching_ (MVInfo * mvi);




#endif // MV_H
