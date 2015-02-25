#include <dirent.h> // directory header
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <time.h>
#include <string.h>
#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"



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
  cleanup();
  return 0; // everything went OK
}
