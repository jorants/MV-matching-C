-------------------------------------------------------------------------
1. The file addzer.f  converts DIMACS format to Rothberg's format so that 
   vertices of degree zero can be input.  
-------------------------------------------------------------------------

-------------------------------------------------------------------------
2. The Match_C.shell file contains a program, written in C, for finding 
  MAXIMUM-CARDINALITY MATCHING in UNDIRECTED graphs.  
-------------------------------------------------------------------------

This program was written by Ed Rothberg to implement H. Gabow's N-cubed 
non-weighted matching algorithm. For an explanation of the algorithm 
see JACM 23, pp221-34 .

MAIN PROGRAMS:
		- match

REQUIRED FILES:
		readme input.form output.form makefile graphtypes.h 
                glib.c main.c match.c

TO GET THESE FILES:
        Run the "match_c.shell" file in the /bin/sh shell. 
		(It will unwrap itself.)
		Or copy the files themselves.

FILE DESCRIPTIONS: 
		input.form:  Describes the input formats for match.
		output.form: Describes output formats for match. 
		test.in:  A sample input for match

                edge_to_wm: A shell file for converting input files in
                   DIMACS .edge format to inputs suitable for match.
                   It uses the following awk files: double.a strip.a 
		 
                   Usage:  edge_to_wm  input.edge output.match

                The others are source code files written in C language.

HOW TO MAKE THE PROBLEM-SOLVERS:

		match:  Solves maximum matching for undirected graphs presented in an 
		       adjacency-list format. 

		       Requires: graphtypes.h glib.c main.c match.c makefile
		
		       To compile: $make
		       To invoke: $match input_file
		
		      If no filename is given, a small help message appears. It always  
		      writes to standard out.  

/ This readme file was written by DIMACS, and based on experiments with the
codes. /
  
       

