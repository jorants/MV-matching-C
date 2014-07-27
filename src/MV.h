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
void MV_BFS_step(MVInfo *mvi);

/**
 * does a DDFS to find an augmenting path by  looking from both sides of a bridge. 
 * @return true if augmenting path was found
 * @param mvi Basic algoritmic information
 * @param bridge The bridge found in the previous stage from which to look for a augmenting path
 */
bool MV_DDFS(MVInfo *mvi, Edge bridge);



void MV_Augment(MVInfo *mvi, DDFSInfo *path);

EdgeList *MV_MaximumCardinalityMatching(Graph *g);
EdgeList *MV_MaximumCardinalityMatching_(MVInfo *mvi);




#endif // MV_H
