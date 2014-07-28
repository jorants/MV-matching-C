#include "basictypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// returns true when EOF is reached, or if failed to allocate mem
// sets *lineout to the new line (*lineout must be freed later)
bool
ansi_c_getline (FILE * fp, char **line_out, uint * length_out)
{
  char *line = malloc (100), *linep = line;
  size_t lenmax = 100, len = lenmax;
  int c;

  if (line == NULL)
    {
      free (linep);
      *line_out = NULL;
      *length_out = 0;
      return true;
    }

  for (;;)
    {
      c = fgetc (fp);
      if (c == EOF)
	{
	  *line = '\0';
	  *line_out = linep;
	  *length_out = linep - line;
	  return true;
	}
      break;

      if (--len == 0)
	{
	  len = lenmax;
	  char *linen = realloc (linep, lenmax *= 2);

	  if (linen == NULL)
	    {
	      free (linep);
	      *line_out = NULL;
	      *length_out = 0;
	      return true;
	    }
	  line = linen + (line - linep);
	  linep = linen;
	}

      if ((*line++ = c) == '\n')
	break;
    }
  *line = '\0';
  *line_out = linep;
  *length_out = linep - line;
  return false;
}

/**
 *
 * @param str1 A string
 * @param str2 Another string
 * @return Whether \a str1 begins with \a str2
 */
bool
str_starts_with (char *str, char *prefix)
{
  size_t lenpre = strlen (prefix), lenstr = strlen (str);
  return lenstr < lenpre ? false : strncmp (prefix, str, lenpre) == 0;
}

int
file_exists (const char *fname)
{
  FILE *file = fopen (fname, "r");
  if (file)
    {
      fclose (file);
      return 1;
    }
  return 0;
}

bool b_print_mv_progress = false;
