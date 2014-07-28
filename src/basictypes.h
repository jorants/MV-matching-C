/**
 @file
 */
#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

typedef unsigned int uint;
#define INFINITY ((unsigned int) -1)

typedef struct
{
  uint v1, v2;
} Edge;

bool ansi_c_getline (FILE * fp, char **line_out, uint * length_out);
bool str_starts_with (char *str, char *prefix);
int file_exists (const char *fname);

bool b_print_mv_progress;

#endif // BASIC_TYPES_H
