#!/bin/bash

# This script copies the NodeList struct and related functions
# and converts them into PointerList and EdgeList structs,
# which are basically identical structures, with different contents

# I.e., this is a poor-man's c++ template

cp NodeList.h PointerList.h
cp NodeList.c PointerList.c

sed -i 's/NODELIST_H/PTRLIST_H/g' PointerList.h
sed -i 's/NodeList/PointerList/g' PointerList.h
sed -i 's/NodeList/PointerList/g' PointerList.c
sed -i 's/LIST_TYPE/PTR_LIST_TYPE/g' PointerList.h
sed -i 's/PTR_LIST_TYPE uint/PTR_LIST_TYPE void*/g' PointerList.h
sed -i 's/LIST_TYPE/PTR_LIST_TYPE/g' PointerList.c

cp NodeList.h EdgeList.h
cp NodeList.c EdgeList.c

sed -i 's/NODELIST_H/EDGELIST_H/g' EdgeList.h
sed -i 's/NodeList/EdgeList/g' EdgeList.h
sed -i 's/NodeList/EdgeList/g' EdgeList.c
sed -i 's/LIST_TYPE/EDGE_LIST_TYPE/g' EdgeList.h
sed -i 's/EDGE_LIST_TYPE uint/EDGE_LIST_TYPE Edge/g' EdgeList.h
sed -i 's/EDGE_LIST_TYPE_EQUALS(x, y) (x == y)/EDGE_LIST_TYPE_EQUALS(x, y) (((x.v1 == y.v1) \&\& (x.v2 == y.v2))||((x.v1 == y.v2) \&\& (x.v2 == y.v1))) /g' EdgeList.h
sed -i 's/LIST_TYPE/EDGE_LIST_TYPE/g' EdgeList.c

