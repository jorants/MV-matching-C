#include <dirent.h> // directory header
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <time.h>
#include <string.h>
#include "libmv.h"




int getmatchnum(char * fn, long * time)
{
   MVInfo *mvi = MVInfo_init_file(fn);
  struct timespec start, stop;
  clock_gettime(CLOCK_MONOTONIC, &start);
  EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  clock_gettime(CLOCK_MONOTONIC, &stop);
  *time = (long)(stop.tv_sec- start.tv_sec) * 1000000LL + (long)(stop.tv_nsec-start.tv_nsec) / 1000LL;

  EdgeListIterator * current = matching->first;
  int siz = 0;
  while(current) {
    current = current-> next;
    siz++;
  }
  // deallocate
  EdgeList_delete(matching);
  Graph *g = mvi->graph;
  MVInfo_delete(mvi);
  Graph_delete(g);
  return siz; 
}


int main () // entry point of the program
{
  // first off, we need to create a pointer to a directory
  DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
  struct dirent *pent = NULL;

  // I used the current directory, since this is one which will apply to anyone reading
  // this tutorial~ If I said "C:\\" and you're on Linux, it may get a little confusing!
  pdir = opendir ("graphs"); // "." will refer to the current directory
  if (pdir == NULL) // if pdir wasn't initialised correctly
    { // print an error message and exit the program
      printf ("\nERROR! pdir could not be initialised correctly\n");
      exit (3);
    } // end if

  while (pent = readdir (pdir)) // while there is still something in the directory to list
    {
      if (pent == NULL) // if pent has not been initialised correctly
        { // print an error message, and exit the program
          printf ("\nERROR! pent could not be initialised correctly\n");
          exit (3);
        }
      // otherwise, it was initialised correctly. let's print it on the console:
      int n,m,a;

      int hits = sscanf(pent->d_name , "%i_%i_%i.txt", &m,&n,&a);

      if(hits == 3){
	char *fullpath = malloc( strlen(pent->d_name) + 9);
	sprintf(fullpath, "graphs/%s",  pent->d_name);
       long time;
       int res =  getmatchnum(fullpath,  &time);
       free(fullpath);
       if(m!=res)
         printf ("%i %i %i %i %li\n", m,n,a,res,time);
      }
    }

  // finally, let's close the directory
  closedir (pdir);

  return 0; // everything went OK
}
