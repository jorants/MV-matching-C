program match(input,output);

(*  This file contains an implementation of 			*)
(*  The Micali-Vazirani Maximum Cardinality Matching Algorithm 	*)
(*  								*)
(*  coded by Steven Crocker					*)
(*  present address: Department of Computer Science,		*)
(*		     University of Chicago			*)
(*								*)
(*    email address: crocker@cs.uchicago.edu			*)
(*								*)
(*								*)
(*  The program expects its input file to be in the DIMACS	*)
(*  Implementation Challenge format for graphs represented by   *)
(*  edge adjacencies.  E.g.  A triangle could be give as below: *)  
(*								*)
(*  c  Comments begin with a c in column 1,			*)
(*  c  The problem statement, begins with a p in column 1 and 	*)
(*  c  has the word "edge" just after the p to specify edge 	*)
(*  c  format.	Following that, the next two columns contain 	*)
(*  c  the number of verices and the number of edges.		*)
(*  c  Edges are given 1 per line, each line begining with an e	*)
(*  c  in column 1 and containing vertex 1 and vertex 2 in the  *)
(*  c  next two columns.  Any addtional information on an edge  *)
(*  c  line is ignored.						*)
(*  c  Here is the triangle:					*)
(*  p  edge 3 3							*)
(*  e  1 2							*)
(*  e  1 3							*)
(*  e  2 3							*)
(* 								*)
(* 								*)
(*  Input is passed to the program through standard input.	*)
(*  The following two lines would run the matching program on 	*)
(*  a file named triangle containing the graph given above.	*)
(*								*)
(*  pc cardmatch.p -o cardmatch					*)
(*  cardmatch < triangle					*)
(*								*)
(*								*)
(*  Details of how the algorithm works can be found in 		*)
(*  S. Micali and V.V. Vazirani, "An O(sqrt(|V|)*|E|) algorithm	*)
(*  for finding maximum matching in general graphs" in Proc.	*)
(*  21st Annual IEEE Symposium on Foundations of Computer 	*)
(*  Science, 1980, pp. 17-27.					*)
(* 								*)


const showmatch = true;  		(* True => print out the mate of each vertex in the matching. *)
      showtime  = false;   		(* True => print out the cardinality of the matching and
						   the number of milliseconds taken to find it. *)
      
     
var 
    numvert, numedges : integer;        (* numvert, numedges are the number of vertices and edges *)
    fmcount  : integer;	(* counts of the number of matched vertices from the two algorithms *)
    starttime,endtime,fasttime : integer;              (* timing quantities *)

    grfilename : packed array [1..100] of char;

    





procedure fastmatch;

type	vertexptr = ^vertex;
	edgeptr = ^edge;
	blossptr = ^bloss;
	levelptr = ^level;

	side = (left,right,none);

	vertex = record
		num	:	integer;		(* vertex number (for debugging purposes) *)
		elist 	: 	edgeptr;		(* header of edge list *)
		mate 	:	vertexptr;		(* mate *)
		medge	:	edgeptr;		(* matched edge *)
		prednx,				(* header of forward list of pred *)
		predpr	:	edgeptr;		(* header of reverse list of pred *)
		predcnt :	integer;		(* predecessor count *)
		blossom	:	blossptr;		(* blossom *)
		markside :	side;			(* side of blossom vertex is on in ddfs *)
		markcall :	integer;		(* call # of blossaug this vtx visited on *)
		llside,
		rlside	:	vertexptr;		(* dbl link list for vertices with same side marking *)
		bstar	:	vertexptr;		(* base-star structure *)
		visblos :	blossptr;		(* blossom searching when visited *)
		visside :	side;			(* side of blossom searching when visited *)
		present :	boolean;
		evlev	:	integer;		(* even level *)
		nxeven	:	vertexptr;		(* next vertex with this even level *)
		odlev	:	integer;		(* odd level *)
		nxodd	:	vertexptr;		(* next vertex with this odd level *)
		level	:	integer;
		anom	:	edgeptr;		(* header of anomalies list for this vertex *)
		lastused :	edgeptr;
		lastvisited : 	edgeptr;
		parvert :	vertexptr;		(* father vertex pointer from the ddfs *)
		paredge :	edgeptr;		(* father edge pointer from the ddfs *)
		pathvert :	vertexptr;		(* a vertex on the path *)
		pathedge :	edgeptr;		(* an edge on the path *)
		estack	:	vertexptr;		(* stack pointer for topological erase *)
		next	:	vertexptr;		(* next vertex in the graph *)
		end;

	edge = record
		num 	:	integer;		(* number of this edge *)
 		v1, v2	:	vertexptr;		(* two vertices defining the edge *)
		nx1, nx2 :	edgeptr;		(* next edges incident to v1 and v2, respectively *)
		prednx	:	edgeptr;		(* prednedge *)
		predpr :	edgeptr;		(* predledge *)
		predvtx :	vertexptr;		(* predvertex *)
		bridnx	:	edgeptr;		(* brid *) (* next bridge on the same level *)
		visblos :	blossptr;		(* vis[].blosm *)
		visside :	side;			(* vis[].side *)
		used	:	boolean;
		isabridge :	boolean;		(* true if this edge is a bridge *)
		nxanom	:	edgeptr;
		next	:	edgeptr;		(* next edge in the graph *)
		end;

	bloss = record
		base 	:	vertexptr;		(* defblos[ ,base] *) (* base vertex of blossom *)
		peake	:	edgeptr;		(* defblos[ ,peake] *) (* peak edge of blossom *)
		end;

	level = record
		levnum	:	integer;		(* the level number *)
		vertices :	vertexptr;		(* head of vertex list of vertices on this level *)
		firstbridge :	edgeptr;		(* first of the bridges on this level *)
		lastbridge :	edgeptr;		(* last of the bridges on this level *)
		next	:	levelptr;		(* next level *)
		end;


var vmin : vertexptr;
    emin : edgeptr;
    inf  : integer;
    goodgraph : boolean;



procedure getgraph (var vmin : vertexptr;
		    var emin : edgeptr;
		    var goodgraph: boolean);
			(*  get the next graph specification *)
                        (*  and initialize the adjacency lists of edges *)


 
                            (* on entry -- the file 'graph' contains the vertex pairs
					   defining the edges of the graph 
                               on exit  -- the vertex and edge nodes have been built and
					   initialized to the adjacency lists of the vertices,
					   where each edge has one record and is on the adjacency
					   list of two vertices.
                            *)

label 1;

type	blockptr = ^block;
	block = record
		verts : array [0..511] of vertexptr;
		next : blockptr;
		end;

var vt1,vt2  :  vertexptr;
    nuedge  : edgeptr;
    nuv1, nuv2 : integer;
    cnt : integer;
    c,d1,d2,d3,d4 : char;
    firstblk, lastblk, vblk : blockptr;




function vrec (v : integer) : vertexptr;

var i, vblknum, vloc : integer;
    blk : blockptr;

begin
   vblknum := v div 512;
   vloc := v mod 512;
   blk := firstblk;
   for i := 1 to vblknum do blk := blk^.next;
   vrec := blk^.verts[vloc];
end;




procedure setupverts ( numverts : integer; var vmin : vertexptr);

var   	max,				(* the number of vertices in the last block *)
	i,				(* the number of the current block *)
	j,				(* the location of the current vertex in the current block *)
	final,				(* the number of the last block needed *)
	loc0num : integer;		(* the vertex number of the vertex in location 0 of a block *)
	lastvert : vertexptr;

begin
   final := numverts div 512;
   loc0num := 0;
   firstblk := nil;
   lastblk := nil;
   lastvert := nil;
   for i := 0 to final do begin
      new(vblk);
      if firstblk = nil then begin
         firstblk := vblk;
         lastblk := vblk;
	 lastblk^.next := nil;
         end
      else begin
	 lastblk^.next := vblk;
	 lastblk := vblk;
	 end;
      if i = final then max := numverts mod 512
      else max := 511;
      with lastblk^ do begin
	 next := nil;
 	 for j := 0 to max do begin
	    new(verts[j]);
            if lastvert = nil then lastvert := verts[j];
	    lastvert^.next := verts[j];
	    lastvert := verts[j];
	    with verts[j]^ do begin
	       elist := nil;
	       num := loc0num + j;
	       mate := nil;
	       medge := nil;
	       next := nil;
	       end;
	     end;
	 loc0num := loc0num + 512;
	 end;
      end;
   vmin := firstblk^.verts[1];
end;




procedure skipwhite(var c : char);

begin
   while not eoln and (c in [' ','	']) do 
      read(c);
end;




function skipjunk(var c : char; d : char): boolean;
		(* Skip ahead to a line where a character the same as d is the first in the line. *)
		(* Assumes c is the first character of the first line to be checked. *)

begin
   skipwhite(c);
   while (c <> d) and not eof do begin
      readln;
      read(c);
      skipwhite(c);
      end;
   skipjunk := not eof;
end;





begin
   goodgraph := false;
   read(c);
   if skipjunk(c,'p') then begin
      read(d1);
      skipwhite(d1);
      if d1 <> 'e' then begin
          writeln(' Error in input format: only edge adjacency graph specifications accepted.');
          goto 1;
          end;
      read(d2,d3,d4);
      read(numvert,numedges);
      setupverts(numvert,vmin);
      emin := nil;
      readln;   (* no more to read from the problem line *)
      cnt := 0;
      while (cnt < numedges) do begin
	 while eoln do readln;
         if eof then begin
            writeln(' Error in graph spec, not enough edges.');
            goto 1;
            end;
         read(c);
         if not skipjunk(c,'e') then begin
            writeln(' Error in graph specification.');
            goto 1;
            end
         else begin
            cnt := cnt+1;
            new(nuedge);
            readln(nuv1,nuv2);
            vt1 := vrec(nuv1);
            vt2 := vrec(nuv2);
            with nuedge^ do begin
               num := cnt;
               v1 := vt1;
               v2 := vt2;
               nx1:= vt1^.elist;			(* insert ce into lvx's and rvx's edge lists *)
               nx2:= vt2^.elist;
               vt1^.elist := nuedge;
               vt2^.elist := nuedge;
               end;
            nuedge^.next := emin;
            emin := nuedge;
            end;		(* else *)
         end;			(* while *)
      goodgraph := true;
      end;
1:
end;





procedure printmatch;
                                  (* print the current matching *)

const tab = '	';
var i : vertexptr;	

begin
   fmcount := 0;
   i := vmin;
   while i <> nil do begin
      if (i^.mate <> nil) then
         fmcount := fmcount + 1;
      i := i^.next;
      end;
   write ('s  ',(fmcount div 2):1);

   if showtime then
	   write(tab,' time=',fasttime:1,' msec., |V|=',numvert:1,', |E|=',numedges:1);

   writeln;

   if showmatch then begin
      i := vmin;
      while i <> nil do begin
         if (i^.mate <> nil) then
	   if i^.mate^.num > i^.num then writeln ('m  ',i^.num:3,' ',i^.mate^.num:3);
         i := i^.next;
         end;
      end;
end;





procedure match (vmin : vertexptr; emin : edgeptr);

(* find a maximal matching for a graph in O(E*sqrt(V)) time *)



type	
        updn = -1..+1;
        evodd = (even, odd);


var 
        serlev,temp : integer;

        strtside : array [side] of vertexptr;
	augmented,augoccur : boolean;
	nx, ce : edgeptr;
	vinit : vertexptr;
        thiscallba : integer;                  (* unique integer identifying this ba call  *)
        cv,cu : vertexptr;
        predve : edgeptr;
	maxlev : levelptr;                (* maxlev is highest nonempty level *)
	dummybloss : blossptr;
	activelevel, nextlevel : levelptr;		(* current and next level of the bfs *)


function findlevel(clevnum : integer):levelptr;
		(* find the record of level number clevnum *)

var i : integer;
    maxlevnum : integer;
    plevptr : levelptr;
    nlev : levelptr;

begin
   maxlevnum := maxlev^.levnum;
   if clevnum > maxlevnum then begin
      plevptr := maxlev;
      for i := maxlevnum + 1 to clevnum do begin
	 new(nlev);
	 plevptr^.next := nlev;
	 plevptr := nlev;
         nlev^.levnum := i;
	 nlev^.vertices := nil;
	 nlev^.firstbridge := nil;
	 nlev^.lastbridge := nil;
         nlev^.next := nil;
	 end;
      maxlev := nlev;      (* set new maximum level to search to. *)
      findlevel := nlev;
      end
   else begin
      nlev := activelevel;
      while nlev^.levnum < clevnum do nlev := nlev^.next;
      findlevel := nlev;
      end;
end;







procedure setlev (eo:evodd; vert:vertexptr; lev:levelptr);
                            (* insert vert on the vertex list for level lev *)
                            (*  on entry -- the evlev/odlev of vert is infinite
                                on exit  -- the evlev/odlev of vert = lev^.levnum,
                                            vert is in the evlev/odlev list for level lev,
                                            vert^.level is the minimum of the even/odd levels of vert
                            *)



begin
   if lev = nil then
      lev := findlevel(activelevel^.levnum + 1);
   with vert^ do
      with lev^ do begin
         case eo of
            even : begin
                      evlev := levnum;		(* vert^.evlev := lev^.levnum *)
                      nxeven := vertices;      (* vert^.nxeven := lev^.vertices *)
                      vertices := vert;		(* lev^.vertices := vert *)
                   end;
            odd :  begin
                      odlev := levnum;		(* vert^.odlev := lev^.levnum *)
                      nxodd := vertices;	(* vert^.nxodd := clev^.vertices *)
                      vertices := vert;		(* lev^.vertices := vert; *)
                   end;
            end;
         if levnum < level then level := levnum;     (* change level of vertex *)
         end;
end;





procedure addbridge (lev : levelptr; edg:edgeptr);
                            (* add edg to bridges(lev) *)

                            (* on entry -- brid(edg) is undefined 
                               on exit  -- brid(edg) = lev,
                                           edg is in the list for bridges on level lev,
                                           maxlev is the highest nonempty level
                            *)


begin
   if lev^.firstbridge = nil then
      lev^.firstbridge := edg
   else 
      lev^.lastbridge^.bridnx := edg;
   lev^.lastbridge := edg;
   edg^.bridnx := nil;
   edg^.isabridge := true;
end; 





procedure addpred (l:vertexptr; e: edgeptr);
                                (* add e to the predecessor list of l *)
                                (* on exit -- e is on the predecessor list of l,
                                              the predecessor count of l has been incremented by 1 *) 

var  fst : edgeptr;

begin
   fst := l^.prednx;
   e^.prednx := fst;
   if fst <> nil then
      fst^.predpr :=e;
   e^.predvtx := l;
   l^.prednx := e;
   e^.predpr := nil;
   l^.predcnt:= l^.predcnt + 1;
end;





procedure addanom (l:vertexptr; e:edgeptr);
                            (* add e to the anomalies list of l *)
                            (* on exit -- e is on the anomalies list of l *)

begin
   e^.nxanom := l^.anom;
   l^.anom := e;
end;





function otherv (v : vertexptr; e : edgeptr) : vertexptr ;
                                      (*  return the vertex linked to v by e *)
begin
   if e^.v1 = v then otherv := e^.v2
      else otherv := e^.v1;
end;








procedure blossaug (w1,w2 : vertexptr; var augmented : boolean ; peakedg : edgeptr);
                                   (* do a double depth first search to find and augment a path or define a blossom *)
                                   (* on entry -- there exists two paths from at least one free vertex 
                                                  to the vertices w1 and w2, neither path includes the
 other vertex.
                                      on exit  -- an augmenting path has been found through w1-peakedg-w2 
                                                  and has been augmented and erased
                                                        or
                                                  no augmenting path of length <= 2*serlev + 1 exists 
                                                  through w1-peakedge-w2 and a new blossom has been 
                                                  formed with w1 and w2 as the peaks
                                                        or
                                                  the paths from free vertices to w1 and w2 are not
                                                  vertex disjoint with an augmenting path of length 2*serlev+1
                                                  previously augmented and erased
                                    *)






var 
    u,ur,ul,vl,vr,w,barrier : vertexptr;
    dcv, bstardcv : vertexptr;
    anomedg,ranedg,lanedg : edgeptr;
    s : side;
    temp : integer;
    bridlevel : levelptr;
    blossomfound : boolean;
    nubloss : blossptr;




procedure connectpath (v1,v2 : vertexptr; eg : edgeptr; dir : updn);
                          (* connect the path list between vertices v1 and v2 *)
                          (* on entry -- v1 and v2 are vertices, eg is an edge incident to one of the vertices
                             on exit  -- if direction is positive then the list in path goes from v2 to v1
                                         otherwise it goes from v1 to v2
                          *)

begin
   if dir = -1 then begin
        v1^.pathvert := v2;
        v1^.pathedge := eg;
        end
      else begin
        v2^.pathvert := v1;
        v2^.pathedge := eg;
        end;
end;






procedure findpath (high,low : vertexptr; b : blossptr;direction : updn; sidesearch:side); forward;





procedure openbl (ent,base : vertexptr; direct : updn); 
                         (* find an alternating path from ent to base filling in all vertices in the blossom of ent *)
                         (* on entry -- ent is the only vertex in the path which is in 
                                       the blossom of ent
                            on exit  -- path has been expanded to contain an alternating path in 
                                        the graph from ent to the base of ent^.blossom 
                         *)
                   


var b : blossptr;
    pe : edgeptr;
    pl,pr : vertexptr;


begin
   b := ent^.blossom;
   if ent^.level mod 2 = 0 then
      findpath (ent,base,b,direct,ent^.markside)
     else begin
      pe := b^.peake;
      pl := pe^.v1;
      pr := pe^.v2;

      if ent^.markside = left then begin
         findpath (pl,ent,b,(direct*-1),left);
         findpath (pr,base,b,direct,right);
         connectpath (pl,pr,pe,-1*direct);
         end
        else begin
         findpath (pr,ent,b,(direct*-1),right);
         findpath (pl,base,b,direct,left);
         connectpath (pr,pl,pe,-1*direct);
         end;
      end;
end;






function bastar (v : vertexptr):vertexptr;
                        (* find the vertex which is the base* of v *)
                        (* on entry -- v is a vertex in the graph 
                           on exit  -- bastar is the base* of v,
                                       the path traversed has been compressed
                        *)

var n, vnxt : vertexptr;

begin
   n := v;
   while n^.bstar <> nil do n := n^.bstar;
   while v <> n do begin
      vnxt := v^.bstar;
      v^.bstar := n;
      v := vnxt;
      end;
   bastar := n;
end;




procedure findpath;

(* procedure findpath (high,low : vertexptr; b : blossptr;direction : updn; sidesearch:side); *)


                         (* Build a path in the graph from (high to low/low to high) 
                            when directon is (+1/-1) using the father tree developed in blossaug *)
                         (* on entry -- high is an ancestor of low in the father tree
                            on exit  -- path contains an ordered list of the vertices in 
                                        the path from high to low, if direction = +1 on entry then the list is 
                                        from high to low, otherwise it is from low to high
                        *)





var e : edgeptr;
    u,v : vertexptr;
    falsebloss : boolean;



begin 
   if high <> low then begin
     falsebloss := false;
     v := high;
     u := v;
     predve := high^.prednx;
     while (u <> low) do begin
        if predve = nil then predve := v^.prednx;
                                          (* Find next unvisited predecessor edge. *)
        while (predve^.visblos = b ) and (predve^.prednx <> nil) do
           predve := predve^.prednx;      
        v^.lastvisited := predve;    (* Update start of unvisited predecessors. *)
                                           (* Act on the kind of predecessor edge found. *)
        if predve^.visblos = b then begin         (* No more unvisited predecessor edges. *)
           v := v^.parvert;                       (* So backup. *)
           predve:=v^.lastvisited;
           end
        else if ((v^.blossom <> b) and falsebloss) then begin  
                                                      (* Wrong path down blossom, reverse and erase. *)
           v := v^.parvert;
           predve:=v^.lastvisited;
           end
        else begin
           falsebloss := false;
           if v^.blossom = b then begin         (* Choose an unvisited predecessor edge. *)
              predve^.visblos := b;                  (* Mark this edge "visited" by the b call to findpath. *)
              predve^.visside := sidesearch;
              u := otherv (v,predve);
              end
           else u := v^.blossom^.base;             (* Pass over the blossom, for now. *)
           if u <> low then
              if ((u^.visside = sidesearch) and (u^.visblos = b))      (* u is 'visited' *)
                 or (u^.level <= low^.level )                            (* or missed low mark => wrong path *)
                 or ((u^.blossom = b) and (u^.markside <> sidesearch))  (* or u is on the other half of path
                                                                            through the blossom *)
                 then begin        (* This edge fails, perhaps because of a wrong blossom, so try again. *)
                 falsebloss := true;
                 end
              else begin           (* Found a good prospect.  Step down it and continue the dfs of findpath. *)
                 u^.visblos := b;
                 u^.visside := sidesearch;
                 u^.parvert := v;
                 u^.paredge := predve;
                 v := u;
                 predve:=v^.lastvisited;
                 end;  (* else *)
        end (* else *)
     end; (* while*)
   
                          (* path found *)
   u^.parvert := v;
   u^.paredge := predve;
   e := u^.paredge;
   while v <> high do begin               (* u = low *)
      connectpath (u,v,e,direction);
      u := v;
      v := v^.parvert;
      e := u^.paredge;
      end;
   connectpath (u,v,e,direction);
   if direction = +1 then begin           (* initialize for traversal of path which opens blossoms  *)
      v := high;
      u := high^.pathvert;
      end
   else begin
      u := low;
      v := low^.pathvert;
      end;
   while not (((direction=+1)and(v=low))or((direction=-1)and(u=high))) do begin     
                                           (* traverse the path in correct direction. *)
      if v^.blossom <> b then openbl (v,u,direction);
      if direction = +1 then begin        (* step down the path *)
         v := u;
         u := u^.pathvert;
         end
      else begin
         u := v;
         v := v^.pathvert;
         end;
      end; (* while *)
   end; (* if high <> low *)
end;  (* findpath *)
 



procedure augment (lv,rv : vertexptr);
                           (*  Augment the path between lv and rv.  *)
                           (* on entry -- path contains an augmenting path from lv to rv
                              on exit  -- the matching along this path has been augmented
                           *)

var mv : vertexptr; 
    e : edgeptr;


begin
   repeat                            (*  Traverse the augmenting path and *)
      mv := lv^.pathvert;         (*  change the matching.  *) 
      e := lv^.pathedge;
      lv^.mate :=mv;  
      lv^.medge :=e;
      mv^.mate :=lv;
      mv^.medge :=e;
      lv := mv^.pathvert;
   until mv = rv;
end;





procedure toperase (vl,vr : vertexptr);
                         (* do a topological erase on the path between vl and vr *)
                         (* on entry -- path contains a path from vl to vr,  all vertices and edges 
                                        on this path are present
                            on exit  -- all vertices on the path are not present, all edges incident 
                                        to a vertex which is no longer present are not present, 
                                        all vertices which have no present edges incident to them are not present   
                         *)

var erasetop : vertexptr;
    es : boolean;
    vert : vertexptr;



procedure pushvert ( v : vertexptr);
                          (* push v onto the stack of vertices to be erased *)

begin
   v^.estack := erasetop;
   erasetop := v;
end;



procedure popvert (var v : vertexptr; var empty : boolean);
                         (* pop v from the stack of vertices to be erased *)

begin
   if erasetop = nil then empty := true
   else begin
      v := erasetop;
      empty := false;
      erasetop := erasetop^.estack;
      v^.estack := nil;
      end;
end;




procedure erase ( v : vertexptr );
                      (* erase vertex v and edges incident to it *)  
                      (* on entry -- v is a vertex in the graph 
                         on exit  -- v and all its incident edges have been erased, neighboring vertices 
                                     which are now unreachable from a free vertex through breadth
                                     first search paths are stacked for erasing, edges incident to v which 
                                     are in the bfs graph are erased from the predecessor lists of 
                                     the neighboring vertices 
                     *)

var e : edgeptr;
    u : vertexptr;

begin
   v^.present := false;                                      (* erase v *)
   e := v^.elist;
   while e <> nil do                              (* for each present edge incident to v *)
    with e^ do begin
      if (predvtx <> nil) and (predvtx <> v) then begin  (* if bfs goes from v thru e to u *)
	if predvtx^.present then begin
            u := predvtx; 
            u^.predcnt := u^.predcnt -1;         (* decrement topological count of u *)
            if (u^.predcnt = 0) then pushvert(u);          (* if u unreachable by any present 
                                                                    bfs path, stack u for erasing *)
               		(* remove e from pred graph *)
	    if prednx <> nil then prednx^.predpr := predpr;
            if predpr <> nil then predpr^.prednx := prednx
	    else u^.prednx := prednx;
            end;
	 end;   
      if v = v1 then e := nx1 else e := nx2;          (* get next edge incident to v *)
      end;
end;






(* procedure toperase (vl,vr : vertexptr)  *)

begin
   erasetop := nil;
   while vl <> vr do begin
      erase (vl);
      vl := vl^.pathvert;
      end;
   erase (vl);
   popvert (vert,es);
   while not es do begin
      erase (vert);
      popvert (vert,es);
      end;
end;





procedure mrkside (vrtx: vertexptr; sd : side);
                            (* mark the vrtx as belonging to the left or right dfs tree *)
                            (* on entry -- vrtx does not belong to either dfs tree
                               on exit  -- vrtx is marked sd and is inserted in the list of vertices on that tree
                            *)

var i: vertexptr;

begin
   i := strtside [sd];
   vrtx^.rlside := i;
   vrtx^.markside := sd;
   strtside [sd] := vrtx;
   vrtx^.llside := nil;
   vrtx^.markcall := thiscallba;
   if i <> nil then i^.llside := vrtx;
end;







procedure rmvmark (vrtx : vertexptr );
                             (* remove vrtx from the bfs tree it is in *)
                             (* on entry -- vrtx belongs to one of the bfs trees
                                on exit  -- vrtx in is neither dfs tree, the call to blossaug it was marked 
                                            is undefined
                             *) 

var  l,r : vertexptr;

begin
 if vrtx <> nil then begin
   l := vrtx^.llside;
   r := vrtx^.rlside;
   if l = nil then strtside [vrtx^.markside] := r
      else l^.rlside := r;
   if r <> nil then r^.llside := l;
   vrtx^.markcall := -1;
   end;
end;



procedure pushleft(var vl,vr : vertexptr; var lanedg : edgeptr);

begin
      if lanedg = nil then lanedg := vl^.prednx;
      while (lanedg^.used) and (lanedg^.prednx <> nil) do 
         lanedg := lanedg^.prednx;      		  (* Find next unused ancestor edge of vl. *)
      vl^.lastused := lanedg;
      if lanedg^.used then          (* vl has no more unused ancestor edges *)
         if vl^.parvert = nil then blossomfound := true  (* create a blossom *)
         else begin
               vl := vl^.parvert;
               lanedg:=vl^.lastused;
               end
      else begin          (* vl has unused ancestor edges *)
            lanedg^.used := true;
            ul := otherv (vl,lanedg);
            ul := bastar (ul);
            if ul^.markcall <> thiscallba then begin
               mrkside (ul,left);
               ul^.parvert:= vl;
               ul^.paredge := lanedg;
               vl := ul;
               lanedg:=vl^.lastused;
               end
            else if (ul = barrier) or (ul <> vr) then begin 
               if (ul = barrier) and (ul=vr) and (dcv=nil) then dcv := ul;
               end
            else begin
               rmvmark (ul);
               mrkside (ul,left);
               vr := vr^.parvert;
               ranedg:=vr^.lastused;
               ul^.parvert := vl;
               ul^.paredge := lanedg;
               vl := ul;
               lanedg:=vl^.lastused;
               dcv := ul;
               end
            end
end;




procedure pushright(var vl,vr : vertexptr; var ranedg : edgeptr);
begin
      if ranedg = nil then ranedg := vr^.prednx;
      while (ranedg^.used) and (ranedg^.prednx <> nil) do 
         ranedg := ranedg^.prednx;     			 (* Find next unused ancestor edge of vr. *)
      vr^.lastused := ranedg;
      if ranedg^.used then           (* vr has no more unused ancestor edges *)
         if vr = barrier then begin
            if vl^.parvert = nil then blossomfound := true
            else begin
               vr := dcv;
               ranedg:=vr^.lastused;
               barrier := dcv;
               rmvmark (vr);            (* remove vr's old mark *)
               mrkside (vr,right);
               vl := vl^.parvert;
               lanedg:=vl^.lastused;
               end;
            end
         else begin
            vr := vr^.parvert;
            ranedg:=vr^.lastused;
            end
      else begin                      (* vr has unused ancestor edges *)
         ranedg^.used := true;
         ur := otherv (vr,ranedg);
         ur := bastar (ur);
         if ur^.markcall <> thiscallba then begin            (* ur is unmarked *)
            mrkside (ur,right);
            ur^.parvert := vr;
            ur^.paredge := ranedg;
            vr := ur;
            ranedg:=vr^.lastused;
            end
           else begin                            (* u is marked *)
            if ur = vl then dcv := ur;
            end
         end
end;






(* procedure blossaug (w1,w2 : vertexptr; var augmented : boolean ; peakedge : edgeptr);   *)
                              (* Either define a new blossom or discover that w1 and w2 are in the *)
			      (* same blossom or find an augmenting path. *)
                              (* On return-- augmented = true if an augmenting path was found   *)



begin
   blossomfound := false;
   augmented := false;
   if w1^.present and w2^.present  (* Neither w1 nor w2 were on an augmenting path previously found *)
      and
      (bastar(w1) <> bastar(w2)) then begin       (* w1 and w2 are not already in the same blossom *)
   
                          (* initialize for double depth first search *)
      vl := bastar(w1);
      vr := bastar(w2);
      lanedg := vl^.lastused;
      ranedg := vr^.lastused;
      thiscallba := thiscallba + 1;
      strtside [left]:=nil;
      strtside [right] := nil;
      mrkside (vl,left);
      mrkside (vr,right);
      vl^.parvert := nil;
      dcv := nil;
      barrier := vr;
   
      while not blossomfound and not augmented do              (* ddfs search  *)
         if (vl^.mate = nil) and (vr^.mate = nil) then begin   (*  vl and vr are free *)
            findpath (w1,vl,nil,-1,left);
            findpath (w2,vr,nil,+1,right);
            connectpath (w1,w2,peakedg,-1);
            augment (vl,vr);
            toperase (vl,vr);
            augmented := true;
            end
         else if vl^.level >= vr^.level then    (* vl,vr not both free and level(vl) <= level(vr) *)
            pushleft(vl,vr,lanedg)
         else            (*  vl,vr not both free and level (vr) > level(vl) *)
            pushright(vl,vr,ranedg);
   
      if blossomfound then begin
         rmvmark(dcv);                            (* dcv is not in the blossom *)
	 new(nubloss);				    (* create a new blossom *)
         nubloss^.base := dcv;           (* assign it a base vertex *)
         nubloss^.peake := peakedg;      (*       and a peak edge *)
         bstardcv := bastar(dcv);
         
         for s := left to right do begin         (* put each vertex marked left or right during *)
                                                 (*      this call in the new blossom *)
            u := strtside [s];
            while u<>nil do begin
               u^.blossom := nubloss;
               u^.bstar := bstardcv;           (* add vertices of blossom to base* tree *) 
               case u^.level mod 2 of
                  0: setlev (odd,u,findlevel(2*serlev + 1 - u^.evlev)); 
                  1:  begin
                      setlev (even,u,findlevel(2*serlev + 1 - u^.odlev));
                      anomedg := u^.anom;
                      while anomedg <> nil do begin
                         w := otherv (u,anomedg);
                         temp := (u^.evlev + w^.evlev) div 2;
			 bridlevel := findlevel(temp);
                         addbridge (bridlevel,anomedg);
                         anomedg^.used := true;
                         anomedg := anomedg^.nxanom;
                         end;   		(*while*)
                      end; 		(* 1: *)
                  end; 		(* cases *)
               u := u^.rlside;
               end; 		(* while u <> nil *)
            end;			(* for s := ... *)
         end;			(* if blossomfound ... *)
      end;  			(* if w1^.present... *)
end;		(* procedure blossaug *)






procedure bfsinitvertices;

var cv : vertexptr;
    zerolev : levelptr;
begin
   zerolev :=findlevel(0);
   cv := vmin;
   while cv <> nil do begin
      with cv^ do begin
         bstar := nil;
         level := inf;
         lastused := nil;
         lastvisited := nil;
         present := true;
         odlev := inf;
         nxodd := nil;
         nxeven := nil;
         blossom := nil;
         predpr := nil;
         prednx := nil;
         anom := nil;
         visside := none;
         visblos := dummybloss;
         if mate = nil then begin
            setlev (even,cv,zerolev);
            predcnt := 1;
            end
         else begin
            evlev := inf;
            predcnt := 0;
            end;
	 end;
      cv := cv^.next;
      end;
end;





procedure bfsinitedges;

var ce : edgeptr;
begin
   ce := emin;
   while ce <> nil do begin
      with ce^ do begin
         visblos := dummybloss;
         visside := none;
         used := false;
         isabridge := false;
         bridnx := nil;
         predvtx := nil;
         end;
      ce := ce^.next;
      end;
end;




procedure bfsinitsearch;

begin
   serlev := -1;
   new(maxlev);
   maxlev^.levnum := -1;
   maxlev^.firstbridge := nil;     (* bridges[maxlev] set to empty set *)
   maxlev^.lastbridge := nil;
   maxlev^.vertices := nil;
   activelevel := maxlev;
   augmented := false;
end;












procedure matchinit;


begin
   inf := numvert + 1;
   new(dummybloss);
   thiscallba := 0;
   vinit := vmin;
   while vinit <> nil do begin
      vinit^.markcall := 0;
      vinit := vinit^.next;
      end;
end;





begin    (* match *)
   matchinit;
   augmented := true;
   while augmented do begin
      bfsinitsearch;
      bfsinitvertices;
      bfsinitedges;
      while not augmented and (serlev < maxlev^.levnum) do begin
         serlev := serlev + 1;               (* increment to next breadth first search level *)
	 activelevel := findlevel(serlev);
	 if serlev < maxlev^.levnum then
            nextlevel := findlevel(serlev+1)
	 else
	    nextlevel := nil;
         cv := activelevel^.vertices;             (* set current vertex *)
         if  serlev mod 2 = 0  then begin      (* search level is even *)
            while  cv <> nil  do begin    (* for each vertex with evenlevel(v) = search levl *)
               ce := cv^.elist;
               while ce <> nil do begin     (* for each unmatched unused neighbor *)
                  cu := otherv(cv,ce);
                  if ce^.v1=cv then         (* get next edge incident to v.  *)
                     nx := ce^.nx1
                  else
                     nx := ce^.nx2;
                  if (cv^.mate <> cu) and not ce^.used then    (* an unmatched, unused neighbor *)
                     if (cu^.evlev < inf) and (not ce^.isabridge) then begin   (* evenlevel u is finite *)
                        temp := (cu^.evlev+cv^.evlev) div 2;  (* add ce to brigdes of level temp *)
                        addbridge (findlevel(temp),ce);
                        end
                     else if not ce^.isabridge then begin
                        if cu^.odlev = inf then setlev (odd,cu,nextlevel);
                        if cu^.odlev = serlev + 1 then
                           addpred (cu,ce)  (* add cv to predecessors of cu *)
                        else if cu^.odlev < serlev then
                           addanom (cu,ce);      (* add cv to anomalies of cu *)
                        end;
                  ce := nx;
                  end;
                cv := cv^.nxeven;           (*  process next vertex on this search level *)
		end;		(* while ... *)
               end       (* even search level and all vertices on this level *)
         else                    (* odd search level *)
            while cv <> nil do begin         (* for all vertices at this level *)
               ce := cv^.medge;
               cu := cv^.mate;
               if (cu^.odlev = serlev) and (not ce^.isabridge) then begin    (* handle bridges *)
                  temp := (cu^.odlev+cv^.odlev) div 2;
                  addbridge (findlevel(temp),ce);         (* add matching edge to bridges(temp) *)
                  end
               else if cu^.odlev = inf then begin        (* handle predecessors  *)
                  setlev (even,cu,nextlevel);
                  addpred (cu,ce);          (* add the matching vertex of v to predecessors u *)
                  end;
               cv := cv^.nxodd;             (* process next vertex on this odd search level *)
               end;             (* while cv <> nil *)
         ce := activelevel^.firstbridge;            
         while ce <> nil do begin               (* for each edge in bridges of this level call blossaug *)
            blossaug (ce^.v1,ce^.v2,augoccur,ce);
            if augoccur then begin
		augmented := true;
		end;
            ce := ce^.bridnx;                 (* next edge is bridges (searchlevel) *)
            end;
         end; 			(* while not augmented and ... *)
      end;				(* while augmented *)
   end;   (* match *)
   







begin	(* fastmatch *)
    getgraph(vmin,emin,goodgraph);
    if goodgraph then begin
        starttime := clock;
	match(vmin,emin);
        endtime := clock;
        fasttime := endtime-starttime;
        printmatch;
	end;
end;    (* fastmatch *)



(************  end O(E*sqrt(V)) matching algorithm  ************)




begin   
   while not eof do
      fastmatch;
end.
