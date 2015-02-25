#include <dirent.h>
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <time.h>
#include <string.h>
#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"



//this implementation with path compression
int getmatchnum(char * fn, int * time)
{
  MVInfo *mvi = MVInfo_init_file(fn);
  mvi->pathc = 1;
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

//this implementation without path compression
int getmatchnum_2(char * fn, int * time)
{
  MVInfo *mvi = MVInfo_init_file(fn);
  mvi->pathc = 0;
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


//fortran implementation
int getmatchnum_3(char * fn, int * time)
{
  struct timespec start, stop;
  char *com = malloc( strlen(fn) + 30);
  sprintf(com, "./match < %s  > /dev/null",  fn);
  clock_gettime(CLOCK_MONOTONIC, &start);
  int succes = system(com);
  clock_gettime(CLOCK_MONOTONIC, &stop);
  *time = (long)(stop.tv_sec- start.tv_sec) * 1000000LL + (long)(stop.tv_nsec-start.tv_nsec) / 1000LL;
  free(com);
  if(succes!=0){
    *time = -1;
  }

  return 0;
}


//pascal implementation
int getmatchnum_4(char * fn, int * time)
{
  struct timespec start, stop;
  char *com = malloc( strlen(fn) + 30);
  sprintf(com, "./match3 < %s  > /dev/null",  fn);
  clock_gettime(CLOCK_MONOTONIC, &start);
  int succes = system(com);
  clock_gettime(CLOCK_MONOTONIC, &stop);
  *time = (long)(stop.tv_sec- start.tv_sec) * 1000000LL + (long)(stop.tv_nsec-start.tv_nsec) / 1000LL;
  free(com);
  if(succes!=0){
    *time = -1;
  }
  return 0;
}


//V^4 implementation
int getmatchnum_5(char * fn, int * time)
{
  struct timespec start, stop;
  char *com = malloc( strlen(fn) + 30);
  sprintf(com, "./edge_to_cm %s tmp.txt > /dev/null",  fn);
  int succes = system("./match2 tmp.txt  > /dev/null");
  clock_gettime(CLOCK_MONOTONIC, &start);
  succes = system("./match2 tmp.txt  > /dev/null");
  clock_gettime(CLOCK_MONOTONIC, &stop);
  *time = (long)(stop.tv_sec- start.tv_sec) * 1000000LL + (long)(stop.tv_nsec-start.tv_nsec) / 1000LL;
  free(com);
  if(succes!=0){
    *time = -1;
  }

  return 0;
}


//new pathcompression
int getmatchnum_6(char * fn, int * time)
{
  MVInfo *mvi = MVInfo_init_file(fn);
  mvi->pathc = 2;
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



//this implementation but removes non working graphs
int getmatchnum_xxx(char * fn)
{
  char *com = malloc( strlen(fn) + 30);
  sprintf(com, "./testgraphs %s  > /dev/null",  fn);
  int succes = system(com);

  free(com);
  if(succes!=0){

    sprintf(com, "error/%s",  fn);
    printf("rename to: %s\n",com);
    printf("%i\n",rename(fn,com));
    return 0;
  }

  return 1;
}

//binear for us
int getmatchnum_bbb(char * fn, int * time)
{
  struct timespec start, stop;
  char *com = malloc( strlen(fn) + 30);
  sprintf(com, "./testgraphs %s  > /dev/null",  fn);
  clock_gettime(CLOCK_MONOTONIC, &start);
  int succes = system(com);
  clock_gettime(CLOCK_MONOTONIC, &stop);
  *time = (long)(stop.tv_sec- start.tv_sec) * 1000000LL + (long)(stop.tv_nsec-start.tv_nsec) / 1000LL;
  free(com);
  if(succes!=0){
    *time = -1;
  }

  return 0;
}



void cleanup(){

  // first off, we need to create a pointer to a directory
  DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
  struct dirent *pent = NULL;


  pdir = opendir ("graphs"); // "." will refer to the current directory
  if (pdir == NULL) // if pdir wasn't initialised correctly
    { // print an error message and exit the program
      printf ("\nERROR! pdir could not be initialised correctly\n");
      exit (3);
    } // end if
  int ant = 0;
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
      if(hits>2){
        char *fullpath = malloc( strlen(pent->d_name) + 9);
        sprintf(fullpath, "graphs/%s",  pent->d_name);
        getmatchnum_xxx(fullpath);
        free(fullpath);

      }
    }

  // finally, let's close the directory
  closedir (pdir);

}


int main () // entry point of the program
{
  //  cleanup();
  int trouble;
  // first off, we need to create a pointer to a directory
  DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
  struct dirent *pent = NULL;


  pdir = opendir ("graphs"); // "." will refer to the current directory
  if (pdir == NULL) // if pdir wasn't initialised correctly
    { // print an error message and exit the program
      printf ("\nERROR! pdir could not be initialised correctly\n");
      exit (3);
    } // end if
  int ant = 0;
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
      if(hits>2){
        char *fullpath = malloc( strlen(pent->d_name) + 9);
        sprintf(fullpath, "graphs/%s",  pent->d_name);
        // printf("%s\n",fullpath);
        int time,time2,time3,time4,time5,time6;
          int res =  getmatchnum(fullpath,  &time);
          res =  getmatchnum_2(fullpath,  &time2);
	  int tmp;
	  // tmp =  getmatchnum_3(fullpath,  &time3);
          // tmp =  getmatchnum_4(fullpath,  &time4);
	  // tmp =  getmatchnum_5(fullpath,  &time5);
          res =  getmatchnum_6(fullpath,  &time6);
	  printf("%i %i %i %i %i %i %i\n", m, n,  a, res, time, time2, time6);
          //printf("%i %i %i\n", time,time2 ,time3);
        free(fullpath);

      }
    }

  // finally, let's close the directory
  closedir (pdir);

  return 0; // everything went OK
}
