#ifndef DIMACS_H
#define DIMACS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void *_DIMACS_read_graph
      (
	FILE *fp,
        void *(*init)(uint, uint), 
        void (*add_edge)(void*, uint, uint),
	bool *success,
        char *failure_message // Should have length >= 1024
      );
      
#endif