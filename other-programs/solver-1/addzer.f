      program addzer
c
c     This program is intended to be used on the output of the
c     shell file edge_to_cm to create a suitable input file for 
c     Rothberg's cardinality matching code match.c
c     
c     Input to edge_to_cm is a file in DIMACS input format.
c     The output file should contain a line for each vertex,
c     followed by its adjacency list.  Unfortunately, lines
c     with degree 0 are not included.
c
c     This program reads the output from edge_to_cm and inserts
c     additional lines of the form '0 v 0 0' whenever v is a 
c     vertex of degree 0.
c
c     To use this program:
c        edge_to_cm data.edge temp 
c        f77 addzer.f
c        a.out < temp > data.match
c     
      integer n,m,i,deg,v,j,u,k
c
c     read the problem line (n=number of vertices, m=2*number of edges)
      read *, n,m 
      print *,n,m,' U'
      i = 1
c     
c     read the i-th vertex line
c
    7 read(*,*,end=9) deg, v
c
c        if the i-th vertex line does not contain vertex i,
c        then add any missing vertices
c
    8    if (v .gt. i) then
            print *, '0 ',i,' 0 0'
            i = i+1
            goto 8
         endif
c        
c        copy line for vertex v to output file
         print *, deg, v, ' 0 0'
c
c        now copy the adjacency list
c
         do 3 j=1,deg
            read *, u,k
            print *,u,k
    3    continue
         i = i+1
         goto 7
c
c     check for missing vertices at eof
c
    9 if (i .le. n) then
         do 13 j=i,n
   13       print *, '0 ',j, '0 0'
      endif
      end

