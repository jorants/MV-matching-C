#include <dirent.h>
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <time.h>
#include <string.h>

#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>

#include <boost/graph/max_cardinality_matching.hpp>

#include <sys/time.h>

#define dt(b,a) ( (double) (a.tv_usec - b.tv_usec) / 1000000 + (double) (a.tv_sec - b.tv_sec))
  



extern "C"{
#include "../src/MV.h"
#include "../src/MVInfo.h"
#include "../src/Graph.h"
}

using namespace boost;





void showhelp(char *name){
  printf("Usage: %s [options]\n",name);
  printf("The options are:\n");
  printf("  -r,--random               Generate a random graph (default)\n");
  printf("  -f,--file {filename}      Use {filename} as an input\n");
  printf("  -v,--size {number}        Make the random graph of size {size}\n");
  printf("  -p,--densness {perc}      {perc}%% of the edges will be added\n");
  printf("  -l,--match-on-load        While loading match as many adges as possible\n");
  printf("  -n,--pure                 Start with an empty matching\n");
  printf("  -a,--match-after-load     After loading \n");
  printf("  -b,--boost                Use the boost library \n");
  printf("  -m,--libmv                Use the libmv library \n");
  printf("  -h,--help                 Show this text\n");  
}


typedef adjacency_list<vecS, vecS, undirectedS> my_graph;
typedef std::vector<graph_traits<my_graph>::vertex_descriptor> my_matching;


int c = 0;
char use_boost = 0;
char use_random = 1;
char * filename = NULL;
int v = 100;
float p = 0.4;
char matchonload = 0;


inline void edge_boost(my_graph * G,my_matching * M,int a,int b){
  add_edge (a, b, *G);
  if(matchonload == 1){

    if((*M)[a] == 0 and (*M)[b] == 0 and ((((*M)[0] !=a or a == 0) and ((*M)[0] !=b or b == 0)))){ //wierd, unmatched is matched to zero?
      (*M)[a] = b;
      (*M)[b] = a;
      c++;
    }
  }
  
}

inline void edge_libmv(Graph *G,MVInfo * mvi,int a,int b){
  Graph_add_edge(G, a, b);
  if(matchonload == 1){
    if((mvi->v_info[a]->matched == UNMATCHED) && (mvi->v_info[b]->matched == UNMATCHED)){
      	    mvi->v_info[a]->matched = b;
	    mvi->v_info[b]->matched = a;
	    c++;
	    mvi->matched_num++;
    }
  }
}


int main (int argc,char** argv) // entry point of the program
{
  int i;

  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-r")==0 || strcmp(argv[i],"--random")==0){
      use_random = 1;
    }else if(strcmp(argv[i],"-f")==0 || strcmp(argv[i],"--file")==0){
      use_random = 0;
      if(i+1 == argc){
	printf("need filename after %s\n",argv[i]);
	return -1;
      }
      filename = argv[i+1];
      i++;
    }else if(strcmp(argv[i],"-v")==0 || strcmp(argv[i],"--size")==0){
      if(i+1 == argc){
	printf("need size after %s\n",argv[i]);
	return -1;
      }
      
      v = atoi(argv[i+1]);
      i++;
    }else if(strcmp(argv[i],"-p")==0 || strcmp(argv[i],"--denseness")==0){
      if(i+1 == argc){
	printf("need densness after %s\n",argv[i]);
	return -1;
      }
      p = atof(argv[i+1]);
      i++;
    }else if(strcmp(argv[i],"-l")==0 || strcmp(argv[i],"--match-on-load")==0){
      matchonload = 1;
    }else if(strcmp(argv[i],"-n")==0 || strcmp(argv[i],"--pure")==0){
      matchonload = 0;
    }else if(strcmp(argv[i],"-a")==0 || strcmp(argv[i],"--match-after-load")==0){
      matchonload = 2;
    }else if(strcmp(argv[i],"-b")==0 || strcmp(argv[i],"--boost")==0){
      use_boost = 1;
    }else if(strcmp(argv[i],"-m")==0 || strcmp(argv[i],"--libmv")==0){
      use_boost = 0;
    }else if(strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0){
      showhelp(argv[0]);
      return 0;
    }else{
      printf("Dont understand: %s\n",argv[i]);
      showhelp(argv[0]);
      return -1;
    }
  }

    // times
  struct timeval  startinit, startload,endload,startmatch,end;
  
  gettimeofday(&startinit, NULL);

  
  uint size, numedge;
  FILE *fp;
  my_graph * gb;
  my_matching *mb;
  Graph *gmv;
  MVInfo *mvi;  
  // get the size
  if(use_random == 0){
     fp = fopen (filename, "r");
     fscanf (fp, "p edge %d %d\n", &size, &numedge);
  }else{
    size = v;
  }

  
  if(use_boost){
    gb = new my_graph(size);
    mb = new my_matching(size);
  }else{
    gmv = Graph_init (size);
    mvi = MVInfo_init (gmv);
    
  }
  
  gettimeofday(&startload, NULL);
  
  if(use_random == 0){
    int i,j,tmp;
    while ((fscanf (fp, "e %i %i %i\n", &i, &j, &tmp)) != EOF)
    {
      if(use_boost) edge_boost(gb,mb,i-1,j-1); else edge_libmv(gmv,mvi,i-1,j-1);
    }
  }else{
    srandom(7); //temp, change this to the time!

    int a,b;
    for(a=0;a<v-1;a++)
      {
	for(b=a+1;b<v;b++)
	  {

	    if(((float)random())/(RAND_MAX) <= p){

	      if(use_boost) edge_boost(gb,mb,a,b); else  edge_libmv(gmv,mvi,a,b);
	    }
	  }
      }
  }
  
  
  gettimeofday(&endload, NULL);
  graph_traits<my_graph>::vertex_iterator ai, a_end;
  if(matchonload == 2){
    if(use_boost){
      printf("Combination of boost and match after load is not supported\n");
      exit(1);
    }else{
      int a,b;
      for(a=0;a<v-1;a++)
	{
	  if((mvi->v_info[a]->matched == UNMATCHED)){
	    NodeListIterator * nli = gmv->edges[a]->first;
	    while(nli){
	      b = nli->value;
	      if((mvi->v_info[b]->matched == UNMATCHED)){
		mvi->v_info[a]->matched = b;
		mvi->v_info[b]->matched = a;
		mvi->matched_num++;
		c++;
		break;
	      }
	      nli = nli->next;
	    }
	  }
	}      
    }
  }

  
  if(!use_boost){
    mvi->stage = -1;
    mvi->output = false;
    MVInfo_next_stage (mvi);
    mvi->pathc = 1;
  }
  gettimeofday(&startmatch, NULL);

  if(use_boost){
    edmonds_maximum_cardinality_matching(*gb, &(*mb)[0]);
  }else{
      EdgeList *matching = MV_MaximumCardinalityMatching_(mvi);
  }

  gettimeofday(&end, NULL);
  
  double msec = dt(startinit,startload);
  printf("init: %f\n",msec);
  msec = dt(startload,endload);
  printf("load: %f\n",msec);
  msec = dt(endload,startmatch);
  printf("afterload: %f\n",msec);
  msec = dt(startmatch,end);
  printf("matching: %f\n",msec);
  
  printf("matched at start: %i\n",c );

  if(use_boost){
    int matchnum = 0;
    graph_traits<my_graph>::vertex_iterator vi, vi_end;
    for(boost::tie(vi,vi_end) = vertices(*gb); vi != vi_end; ++vi){
      if ((*mb)[*vi] != graph_traits<my_graph>::null_vertex() && *vi < (*mb)[*vi]) {
	matchnum++;
      }
    }
    printf("matched: %i\n",matchnum);
  }else{
    printf("matched: %i\n",mvi->matched_num);
    Graph *gres = mvi->graph;
    MVInfo_delete(mvi);
    Graph_delete(gres);

  }
  
  return 0; // everything went OK
}
