/** @file
 * Contains the main algorithem
 *
 */
#ifndef MV_H
#define MV_H

#include "basictypes.h"
#include "MVInfo.h"

void MV_BFS_step(MVInfo *mvi);

/**
 * Goes through every edge in the current level nodes,
 * assigns new minlevels and discovers next level nodes,
 * attributes predecessors, and marks edges as bridges
 */
bool MV_DDFS(MVInfo *mvi, Edge bridge);

void MV_Augment(MVInfo *mvi, DDFSInfo *path);

EdgeList *MV_MaximumCardinalityMatching(Graph *g);
EdgeList *MV_MaximumCardinalityMatching_(MVInfo *mvi);



#endif // MV_H
