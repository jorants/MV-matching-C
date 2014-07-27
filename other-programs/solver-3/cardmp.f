C*********************************************************************
      PROGRAM CARDMP
C
C     SOLVES THE CARDINALITY MATCHING PROBLEM USING AN
C     IMPLEMENTATION OF THE MICALI-VAZIRANI ALGORITHM
C
C     LANGUAGE:   FORTRAN 77
C
C     PARAMETERS:
C             N = maximum number of nodes allowed
C             M = maximum number of edges allowed
C
C             These parameters are used to determine the amount of
C             memory to be allocated.  Identical parameter statements
C             are included in each of the following modules:
C                 PROGRAM CARDMP  (main program)
C                 SUBROUTINE SEARCH
C                 SUBROUTINE BLSAUG
C                 SUBROUTINE FNDPTH
C
C     INPUT:  Input is read from UNIT 5.
C           * Comment lines begin with 'c' and are ignored.
C             WARNING: This program deviates slightly from DIMACS format.
C             Comments may occur before or after the 'p' line, but
C             no comment may occur after the first 'e' line.
C           * The 'p' line contains the string 'edge' followed by
C             two integers NI and MI.  NI = number of vertices input.
C             MI = number of edges input.  
C           * 'e' lines contain 3 integers representing the two vertices
C             making up an edge, along with the edge weight.  (This 
C             program ignores the edge weights).    
C
C     Warning:  Some versions of FORTRAN may require formatted input.
C               Alternative input statements are provided as comments
C               beginning with 'CI'
C
C     OUTPUT: Output is written to UNIT 6.
C           * Line 1 begins with 's' (solution line) and contains a
C             single integer representing the cardinality of an
C             optimal matching.
C           * Lines 2-NI+1 begin with 'm' (matching line) followed by
C             two integers:  U and MATCH(U).
C             U is a vertex and MATCH(U) is the vertex adjacent
C             to U in the optimal matching or 0 if U is unmatched.
C
C     The main program, Step -1 of SEARCH and subroutine SSORT are
C     based on code written by U. Derigs and G. Kazakidis published in
C
C             R. E. Burkard and U. Derigs.  Assignment and Matching
C             Problems:  Solution Methods and FORTRAN-Programs.  Lecture
C             Notes in Economics and mathematical Systems, Vol. 184,
C             Springer-Verlag, Berlin, 1980.
C
C     Subroutines SEARCH, BLSAUG and FNDPTH are based on an algorithm
C     published in:
C
C             Micali, S. and Vazirani, V. V.,"An O(square root of V * E)
C             Algorithm for Finding Maximum Matching in General Graphs",
C             Proc. 21st Annual Symposium on Foundation of Computer
C             Science, IEEE, 1980, pp. 17-27.
C
C     Programmers:
C              R. Bruce Mattingly  (FR132601@ysub.ysu.edu)
C              Nathan P. Ritchey   (nate@macs.ysu.edu)
C              Department of Mathematical and Computer Sciences
C              Youngstown State University
C              Youngstown, OH  44555
C
C     Version 1.11
C     Last modification:  02/14/92
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
      INTEGER N, M, NP1, MT2
      PARAMETER (N=8192, M=32768)
      PARAMETER (MT2=M*2, NP1=N+1)
C
      CHARACTER CH*1, JUNK*5
      INTEGER I, J, P, U, V, B
      INTEGER DEGREE(N), NBRLL(MT2, 2), PTR(N)
C
      INTEGER NI, MI, MATCH(N), CARD, NINDEX(NP1), NGHBR(MT2)
      COMMON NI, MI, MATCH, CARD, NINDEX, NGHBR
C-----------------------------------------------------------------------
C
C     READ AND CHECK PROBLEM DIMENSIONS
C  
      CH = 'c'
    1 IF (CH .NE. 'p') THEN
         READ(5,*) CH
CI       READ(5, 501) CH
CI501    FORMAT(A1, A5, 2I10)
         GOTO 1
      ENDIF
      BACKSPACE(5)
      READ(5,*) CH, JUNK, NI, MI
CI    READ(5, 501) CH, JUNK, NI, MI
C
      IF (N .LT. NI) THEN
         WRITE(6, 2) 'N', N, NI
    2    FORMAT(1X, 'MAIN:  PARAMETER ', A1,' =',I10,
     +          ' IS TOO SMALL - INPUT VALUE =',I10)
         STOP
      ELSE IF (M .LT. MI) THEN
         WRITE(6, 2) 'M', M, MI
         STOP
      ENDIF
C
    3 IF (CH .NE. 'e') THEN
         READ(5,*) CH
CI       READ(5,501) CH
         GOTO 3
      ENDIF
      BACKSPACE(5)
C
C   INPUT EDGES, STORE ADJACENCY LISTS IN NBRLL (NEIGHBOR LINKED LISTS)
C
      DO 8 I = 1, NI
         DEGREE(I) = 0
         PTR(I) = 0
    8 CONTINUE
      P = 0
      DO 10 I = 1, MI
         READ(5, *) CH, U, V
CI       READ(5,502) CH, U, V
CI502    FORMAT(A1,1X,2I10)
         P = P+1
         NBRLL(P,1) = V
         NBRLL(P,2) = PTR(U)
         PTR(U) = P
         DEGREE(U) = DEGREE(U) + 1
         P = P + 1
         NBRLL(P,1) = U
         NBRLL(P,2) = PTR(V)
         PTR(V) = P
         DEGREE(V) = DEGREE(V) + 1
   10 CONTINUE
C
C     NOW STORE ADJACENCY LISTS SEQUENTIALLY
C
      NINDEX(1) = 1
      DO 12 I = 1, NI
         NINDEX(I+1) = NINDEX(I) + DEGREE(I)
   12 CONTINUE
      DO 16 I = 1, NI
         P = PTR(I)
         DO 14 J = NINDEX(I), NINDEX(I+1)-1
            NGHBR(J) = NBRLL(P,1)
            P = NBRLL(P,2)
   14    CONTINUE
   16 CONTINUE
C
C     CALL SEARCH TO FIND OPTIMAL MATCHING AND CARDINALITY
C
      CALL SEARCH(DEGREE)
C
C     DIMACS OUTPUT
C
      WRITE(6,60)  CARD
   60 FORMAT('s  ', I10)
C
      DO 68 J = 1 , NI
      WRITE(6,66)  J, MATCH(J)
   66 FORMAT('m  ', 2I10)
   68 CONTINUE
C
  100 STOP
      END
C***********************************************************************
      SUBROUTINE SEARCH(DEGREE)
C
C     PERFORMS BREADTH-FIRST SEARCHES FROM FREE (UNMATCHED) VERTICES.
C     IDENTIFIES BRIDGES (EDGES JOINING ALTERNATING PATHS FROM 
C     FREE VERTICES)
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
C     PARAMETERS
C
      INTEGER N, M, NP1, MT2, INFTY, UNDEF, ND2
      PARAMETER (N=8192, M=32768)
      PARAMETER (NP1=N+1, MT2=M*2, INFTY=NP1, UNDEF=0, ND2=N/2)
C
C     ARGUMENTS
C
      INTEGER  DEGREE(*)
C
C     LOCAL VARIABLES
C
      INTEGER B, U, V, E, I, UU, VV, TEMP, NEXT, NLEVI, LEVELI(N), CODE
      INTEGER VP1, MEM(N)
      LOGICAL DIDAUG
C
C     COMMON STORAGE
C
      INTEGER NI, MI, MATCH(N), CARD, NINDEX(NP1), NGHBR(MT2)
      COMMON NI, MI, MATCH, CARD, NINDEX, NGHBR
C
      CHARACTER*1 MARK(N)
      COMMON /BLK0/ MARK
C
      INTEGER EVLEV(N), ODLEV(N), BLOSS(N)
      COMMON /BLK1/ EVLEV, ODLEV, BLOSS
C
      INTEGER ANOM(2,N), AINDEX(N), AFREE
      COMMON /BLK2/ ANOM, AINDEX, AFREE
C
      LOGICAL VISITD(N)
      COMMON /BLK3/ VISITD
C
      INTEGER BASE(ND2), BSTAR(ND2), PEAKL(ND2), PEAKR(ND2),
     +        F(N), PATH(N)
      COMMON /BLK4/ BASE, BSTAR, PEAKL, PEAKR, F, PATH
C
      INTEGER BRIDGE(3,M), BINDEX(ND2), BFREE
      COMMON /BLK5/ BRIDGE, BINDEX, BFREE
C
      INTEGER PRED(2,M), PINDEX(N), PFREE, DERP(2,M), DINDEX(N), DFREE,
     +        PRDCTR(N)
      COMMON /BLK6/ PRED, PINDEX, PFREE, DERP, DINDEX, DFREE, PRDCTR
C
      INTEGER EDGE(MT2)
      COMMON /BLK7/ EDGE
C
      INTEGER LR(N), CALLS
      COMMON /BLK9/ LR, CALLS
C-----------------------------------------------------------------------
      IF (N .LT. NI) THEN
         WRITE(6, 4) 'N', N, NI
    4    FORMAT(1X, 'SEARCH:  PARAMETER ', A1,' =',I10,
     +          ' IS TOO SMALL - INPUT VALUE =',I10)
         STOP
      ELSE IF (M .LT. MI) THEN
         WRITE(6, 4) 'M', M, MI
         STOP
      ENDIF
C 
C  *** STEP -1 ***  Compute initial matching 
C
C     INITIALIZE MATCH ARRAY
C
      DO 20 V = 1, NI
         MATCH(V) = 0
         MEM(V) = V
   20 CONTINUE
      CARD = 0
C
      CALL SSORT(DEGREE, MEM, NI)
      DO 55 VV = 1, NI
         V = MEM(VV)
         IF (MATCH(V) .EQ. 0) THEN
            DO 53 UU = NINDEX(V), NINDEX(V+1)-1
               U = NGHBR(UU)
               IF (MATCH(U) .EQ. 0) THEN
                  MATCH(U) = V
                  MATCH(V) = U
                  CARD = CARD + 1
                  GOTO 55
               ENDIF
   53       CONTINUE
         ENDIF
   55 CONTINUE
      IF (CARD .EQ. NI/2) RETURN
C
C *** STEP 0 *** Initialization
C
    5 DO 10 V = 1, NI
         EVLEV(V) = INFTY
         ODLEV(V) = INFTY
         BLOSS(V) = UNDEF
         PINDEX(V) = 0
         DINDEX(V) = 0
         AINDEX(V) = 0
         VISITD(V)   = .FALSE.
         LR(V) = 0
         MARK(V) = ' '
         F(V) = 0
         PRDCTR(V) = 0
   10 CONTINUE
      PFREE = 1
      DFREE = 1
      AFREE = 1
C
      DO 12 E = 1, MI*2
   12    EDGE(E) = 0
C
      DO 14 V = 0, NI/2
   14    BINDEX(V) = 0
      BFREE = 1
C
      DO 16 V = 1, MI-1
         VP1 = V + 1
         PRED(1,V) = VP1
         DERP(1,V) = VP1
   16 CONTINUE
      PRED(1, MI) = 0
      DERP(1, MI) = 0
      DO 17 V = 1, NI - 1
         VP1 = V + 1
         ANOM(1,V) = VP1
   17 CONTINUE
      ANOM(1, NI) = 0
C
      DO 18 V = 1, MI-1
            BRIDGE(1,V) = V+1
   18 CONTINUE
      BRIDGE(1,MI) = 0
C
      I = -1
      DIDAUG = .FALSE.
      CALLS = 0
      B = 0
C
C *** STEP 1 ***  Check for free vertices
C
      DO 101 V = 1, NI
         IF (MATCH(V) .EQ. 0) EVLEV(V) = 0
  101 CONTINUE
C
C *** STEP 2 ***  Proceed to next level (I) of search
C
  200 I = I + 1
      IF (I .GT. N/2) THEN
          PRINT *, 'SEARCH:  SEARCH LEVEL LIMIT EXCEEDED'
          STOP
      ENDIF
      NLEVI = 0
C
C     Loop 210 searches for vertices with level=I
C
      DO 210 V = 1, NI
         IF ((EVLEV(V) .EQ. I) .OR. (ODLEV(V) .EQ. I)) THEN
C        IF (MIN(EVLEV(V), ODLEV(V)) .EQ. I) THEN
            NLEVI = NLEVI + 1
            LEVELI(NLEVI) = V
         ENDIF
  210 CONTINUE
C
C     If no more vertices have level I then HALT (return to main)
C
      IF (NLEVI .EQ. 0)  RETURN
C
C     Is search level I even or odd?
C
      IF (MOD(I,2) .EQ. 0) THEN
C
C *** STEP 3 ***  For even search levels
C
         DO 310 VV = 1, NLEVI
            V = LEVELI(VV)
            IF ((I .EQ. 0) .AND. (MATCH(V) .NE. 0)) GOTO 310
C
C           Loop 320 processes all neighbors of current V
C
            DO 320 UU = NINDEX(V), NINDEX(V+1)-1
               U = NGHBR(UU)
C
C              EDGE UV IS UNUSED IF CODE IS EVEN
C
               CALL STATUS(U, V, EDGE, CODE, 'R', NINDEX, NGHBR)
               IF ((MATCH(U) .NE. V) .AND. (MOD(CODE,2) .EQ. 0)) THEN
                  IF (EVLEV(U) .NE. INFTY) THEN
                     TEMP = (EVLEV(U) + I) / 2
                     IF ((EVLEV(U) .NE. I) .OR. (U .LT. V)) THEN
                        CALL ADDBRG(U, V, TEMP, BRIDGE, BINDEX, BFREE)
                     ENDIF
                  ELSE
                     IF (ODLEV(U) .EQ. INFTY) ODLEV(U) = I + 1
                     IF (ODLEV(U) .EQ. I + 1) THEN
                        CALL ADDSTK(PRED, PINDEX, PFREE, U, V)
                        CALL ADDSTK(DERP, DINDEX, DFREE, V, U)
                        PRDCTR(U) = PRDCTR(U) + 1
                     ELSE IF (ODLEV(U) .LT. I) THEN
                        CALL ADDSTK(ANOM, AINDEX, AFREE, U, V)
                     ENDIF
                  ENDIF
               ENDIF
  320       CONTINUE
  310    CONTINUE
C
      ELSE
C
C *** STEP 4 ***  For odd search levels
C
         DO 410 VV = 1, NLEVI
            V = LEVELI(VV)
            IF (BLOSS(V) .EQ. UNDEF) THEN
               U = MATCH(V)
               IF (ODLEV(U) .EQ. I) THEN
                  IF (U .LT. V) THEN
                     CALL ADDBRG(U, V, I, BRIDGE, BINDEX, BFREE)
                  ENDIF
               ELSE IF (ODLEV(U) .EQ. INFTY) THEN
C
C                   MAKE V THE ONLY PREDECESSOR OF U
C
                    EVLEV(U) = I + 1
                    NEXT = PINDEX(U)
C
C                   CHECK FOR ERROR CONDITION
C
                    IF (NEXT .NE. 0) PRINT *, 'WARNING FROM SEARCH: ',
     +              U, ' SHOULD NOT HAVE ANY PREDECESSORS'
  408               IF (NEXT .NE. 0) THEN
                       PINDEX(U) = PRED(1, NEXT)
                       PRED(1, NEXT) = PFREE
                       PFREE = NEXT
                       NEXT = PINDEX(U)
                       GOTO 408
                    ENDIF
C
                    CALL ADDSTK(PRED, PINDEX, PFREE, U, V)
                    CALL ADDSTK(DERP, DINDEX, DFREE, V, U)
                    PRDCTR(U) = PRDCTR(U) + 1
               ENDIF
            ENDIF
  410    CONTINUE
      ENDIF
C
C *** STEP 5 ***
C
      NEXT = BINDEX(I)
  510 IF (NEXT .NE. 0) THEN
         U = BRIDGE(2, NEXT)
         V = BRIDGE(3, NEXT)
         NEXT = BRIDGE(1, NEXT)
         IF ((MARK(U) .EQ. 'E') .OR. (MARK(V) .EQ. 'E')) GOTO 510
         IF ((BLOSS(U) .NE. UNDEF) .AND. (BLOSS(V) .EQ. BLOSS(U)))
     +      GOTO 510
         CALL BLSAUG(U, V, DIDAUG, B, I)
         IF (CARD .EQ. NI/2) RETURN
         GOTO 510
      ENDIF
      IF (DIDAUG) THEN
         GOTO 5
      ELSE
         GOTO 200
      ENDIF
C
      END
C***********************************************************************
      SUBROUTINE BLSAUG(W1, W2, DIDAUG, B, I)
C
C     FINDS AN AUGMENTING PATH CONTAINING BRIDGE W1-W2 OR CREATES A NEW
C     BLOSSOM.  THE COMMON VARIABLE CALLS IS INCREMENTED EACH TIME THIS
C     ROUTINE IS CALLED.  I IS THE SEARCH LEVEL.  B COUNTS BLOSSOMS.
C
C     DEPTH-FIRST SEARCHES ARE CONDUCTED SIMULTANEOUSLY FROM W1 AND W2.
C     THE LEFT SEARCH STARTS AT W1 AND THE RIGHT SEARCH STARTS AT W2.
C     LR(N) = +CALLS IF VERTEX N IS VISITED BY THE LEFT SEARCH.
C     LR(N) = -CALLS IF VERTEX N IS VISITED BY THE RIGHT SEARCH.
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
C     PARAMETERS
C
      INTEGER N, M, NP1, MT2, UNDEF, ND2
      PARAMETER (N=8192, M=32768)
      PARAMETER (NP1=N+1, MT2=M*2, UNDEF=0, ND2=N/2)
C
C     ARGUMENTS
C
      INTEGER W1, W2, B, I
      LOGICAL DIDAUG
C
C     LOCAL VARIABLES
C
      INTEGER VL, VR, DCV, BARIER, P1, P2, U, V, TEMP, NEXT, 
     +        INDEX, MEMBER(N), NMEM, UU, LINDEX, RINDEX,
     +        ESTK(N), ETOP, LJUMP, RJUMP
C
C     COMMON STORAGE
C
      INTEGER NI, MI, MATCH(N), CARD, NINDEX(NP1), NGHBR(MT2)
      COMMON NI, MI, MATCH, CARD, NINDEX, NGHBR
C
      CHARACTER*1 MARK(N)
      COMMON /BLK0/ MARK
C
      INTEGER EVLEV(N), ODLEV(N), BLOSS(N)
      COMMON /BLK1/ EVLEV, ODLEV, BLOSS
C
      INTEGER ANOM(2,N), AINDEX(N), AFREE
      COMMON /BLK2/ ANOM, AINDEX, AFREE
C
      INTEGER BASE(ND2), BSTAR(ND2), PEAKL(ND2), PEAKR(ND2),
     +        F(N), PATH(N)
      COMMON /BLK4/ BASE, BSTAR, PEAKL, PEAKR, F, PATH
C
      INTEGER BRIDGE(3,M), BINDEX(ND2), BFREE
      COMMON /BLK5/ BRIDGE, BINDEX, BFREE
C
      INTEGER PRED(2,M), PINDEX(N), PFREE, DERP(2,M), DINDEX(N), DFREE,
     +        PRDCTR(N)
      COMMON /BLK6/ PRED, PINDEX, PFREE, DERP, DINDEX, DFREE, PRDCTR
C
      INTEGER EDGE(MT2)
      COMMON /BLK7/ EDGE
C
      INTEGER LR(N), CALLS
      COMMON /BLK9/ LR, CALLS
C-----------------------------------------------------------------------
      CALLS = CALLS + 1
      IF (N .LT. NI) THEN
         WRITE(6, 4) 'N', N, NI
    4    FORMAT(1X, 'BLSAUG:  PARAMETER ', A1,' =',I10,
     +          ' IS TOO SMALL - INPUT VALUE =',I10)
         STOP
      ELSE IF (M .LT. MI) THEN
         WRITE(6, 4) 'M', M, MI
         STOP
      ENDIF
C
C *** STEP 0 ***
C
      VL = W1
      LJUMP=0
      IF ( BLOSS(W1) .NE. UNDEF) THEN
         CALL BASTAR(0, VL, BSTAR, F, BLOSS)
      ENDIF
C
      VR = W2
      RJUMP=0
      IF (BLOSS(W2) .NE. UNDEF) THEN
         CALL BASTAR(0, VR, BSTAR, F, BLOSS)
      ENDIF
C
C     QUIT IF AN EMPTY BLOSSOM IS DETECTED
C
      IF (VL .EQ. VR) GOTO 500
C
      LINDEX = PINDEX(VL)
      RINDEX = PINDEX(VR)
      LR(VL) = CALLS
      LR(VR) = -CALLS
      MEMBER(1) = VL
      MEMBER(2) = VR
      NMEM = 2
      F(W1) = UNDEF
      DCV = UNDEF
C     IN ORIGINAL ALGORITHM W2 IS REPLACED BY VR BELOW
      BARIER = W2
C
C *** STEP 1.1 ***  If VL and VR are both free vertices
C
  100 IF ((MATCH(VL) .EQ. 0) .AND. (MATCH(VR) .EQ. 0) ) THEN
         CALL FNDPTH(W1, VL, UNDEF, -1)
         CALL FNDPTH(W2, VR, UNDEF, 1)
C
C        Concatenate paths
C
         PATH(W1) = W2
C
C        AUGMENT MATCHING 
C
         P1 = VL
  111    P2 = PATH(P1)
         MATCH(P1) = P2
         MATCH(P2) = P1
         ETOP = ETOP + 1
         ESTK(ETOP) = P1
         ETOP = ETOP + 1
         ESTK(ETOP) = P2
         P1 = PATH(P2)
         IF (P2 .NE. VR) GOTO 111
         CARD = CARD + 1
         DIDAUG = .TRUE.
C
C        PERFORM TOPOLOGICAL ERASE AND RETURN
C
  122    IF (ETOP .GT. 0) THEN
            P1 =  ESTK(ETOP)
            ETOP = ETOP - 1
            IF (MARK(P1) .NE. 'E') THEN
               MARK(P1) = 'E'
               NEXT = DINDEX(P1)
  127          IF (NEXT .NE. 0) THEN
                  P2 = DERP(2, NEXT)
                  NEXT = DERP(1, NEXT)
                  PRDCTR(P2) = PRDCTR(P2) - 1
                  IF (PRDCTR(P2) .EQ. 0) THEN
                     ETOP = ETOP + 1
                     ESTK(ETOP) = P2
                  ENDIF
                  GOTO 127
               ENDIF
            ENDIF
            GOTO 122
         ENDIF
         GOTO 500
C
C *** STEP 1.2 ***  If VL and VR are not both free
C
      ELSE
         IF (MIN(EVLEV(VL),ODLEV(VL)).GE.MIN(EVLEV(VR),ODLEV(VR))) THEN
            GOTO 200
         ELSE
            GOTO 300
         ENDIF
      ENDIF
C
C Does VL have unused ancestor edges?
C
  200 IF (LINDEX .EQ. 0) THEN
         U = 0
      ELSE
         CALL ANCEST(VL, 1, U, LINDEX, PRED, EDGE, MARK, NINDEX, NGHBR)
      ENDIF
      IF (U .EQ. 0) THEN
C
C *** STEP 2.1 ***  VL has no more unused ancestors
C
         IF (F(VL) .EQ. UNDEF) THEN
            IF (DCV .NE. UNDEF) THEN
               GOTO 400
            ELSE
               PRINT *, 'WARNING - BLSAUG(',W1,',',W2,') QUITTING ', 
     +                  'BECAUSE DCV UNDEFINED AT STEP 2.1'
               GOTO 500
            ENDIF
         ELSE
            VL = F(VL)
            LINDEX = PINDEX(VL)
            GOTO 100
         ENDIF
      ELSE
C
C ***  STEP 2.2 *** VL has some unused ancestors
C
C        MARK VL-U 'USED'
C
         CALL STATUS(VL, U, EDGE, 1, 'W', NINDEX, NGHBR)
         LJUMP = VL
         IF (BLOSS(U) .NE. UNDEF) THEN
            CALL BASTAR(VL, U, BSTAR, F, BLOSS)
            LINDEX = PINDEX(VL)
         ENDIF
C
C    *** Is U marked? ***
C
         IF (ABS(LR(U)) .NE. CALLS) THEN
C
C        *** STEP 2.2 (a) ***   U is unmarked
C
            LR(U) = CALLS
            CALL ADDMEM(MEMBER, NMEM, U)
            F(U) = VL
            VL = U
            LINDEX = PINDEX(VL)
            GOTO 100
         ELSE
C
C        *** STEP 2.2 (b) ***   U is marked
C
            IF ((U .EQ. BARIER) .OR. (U .NE. VR)) THEN
               GOTO 100
            ELSE
               LR(U) = CALLS
               CALL ADDMEM(MEMBER, NMEM, U)
               VR = F(VR)
               IF (RJUMP .NE. 0) VR = RJUMP
               RINDEX = PINDEX(VR)
               F(U) = VL
               VL = U
               LINDEX = PINDEX(VL)
               DCV = U
               GOTO 100
            ENDIF
         ENDIF
      ENDIF
C
C Does VR have unused ancestor edges?
C
  300 IF (RINDEX .EQ. 0) THEN
         U = 0
      ELSE
         CALL ANCEST(VR, 1, U, RINDEX, PRED, EDGE, MARK, NINDEX, NGHBR)
      ENDIF
      IF (U .EQ. 0) THEN
C
C    *** STEP 3.1 ***  VR has no more unused ancestors
C
         IF (VR .EQ. BARIER) THEN
            IF (DCV .EQ. 0) THEN 
            PRINT *, 'WARNING - BLSAUG(',W1,',',W2,') QUITTING ', 
     +               'BECAUSE DCV UNDEFINED AT STEP 3.1'
               GOTO 500
            endif
            VR = DCV
            RINDEX = PINDEX(VR)
            BARIER = DCV
            LR(VR) = -CALLS
            CALL ADDMEM(MEMBER, NMEM, VR)
            IF (F(VL) .EQ. UNDEF) THEN
                GOTO 400
            ELSE
               VL = F(VL)
               IF (LJUMP .NE. 0) VL = LJUMP
               LINDEX = PINDEX(VL)
            ENDIF
         ELSE
            VR = F(VR)
            RINDEX = PINDEX(VR)
         ENDIF
         GOTO 100
      ELSE
C
C     *** STEP 3.2 ***  VR has some unused ancestors
C
C        MARK VR-U 'USED'
C
         CALL STATUS(VR, U, EDGE, 1, 'W', NINDEX, NGHBR)
         RJUMP = VR
         IF (BLOSS(U) .NE. UNDEF) THEN
            CALL BASTAR(VR, U, BSTAR, F, BLOSS)
            RINDEX = PINDEX(VR)
         ENDIF
C
C    *** Is U unmarked? ***
C
         IF (ABS(LR(U)) .NE. CALLS) THEN
C
C        *** STEP 3.2 (a) ***
C
            LR(U) = -CALLS
            CALL ADDMEM(MEMBER, NMEM, U)
            F(U) = VR
            VR = U
            RINDEX = PINDEX(VR)
            GOTO 100
         ELSE
C
C        *** STEP 3.2 (b) ***
C
            IF (U .EQ. VL) THEN
               DCV = U
            ENDIF
            GOTO 100
         ENDIF
      ENDIF
C
C *** STEP 4 ***  
C
  400 LR(DCV) = 0
C
C     create new blossom using all vertices marked L or R during call
C
      B = B + 1
      IF (B .GT. ND2) THEN  
         PRINT *,'BLSAUG: OUT OF STORAGE FOR BASE, BSTAR, PEAKL, PEAKR' 
         STOP
      ENDIF
      DO 425 UU = 1, NMEM
         U = MEMBER(UU)
         IF (U .EQ. DCV) GOTO 425
         IF (BLOSS(U) .EQ. UNDEF) THEN
            BLOSS(U) = B
            IF (EVLEV(U) .LT. ODLEV(U)) THEN
               ODLEV(U) = 2 * I + 1 - EVLEV(U)
            ELSE
               EVLEV(U) = 2 * I + 1 - ODLEV(U)
               INDEX = AINDEX(U)
  421          IF (INDEX .NE. 0) THEN
                  V = ANOM(2, INDEX)
                  INDEX = ANOM(1, INDEX)
                  TEMP = (EVLEV(U) + EVLEV(V))/2
                  CALL ADDBRG(U, V, TEMP, BRIDGE, BINDEX, BFREE)
C
C                 MARK U-V 'USED'
C
                  CALL STATUS(U, V, EDGE, 1, 'W', NINDEX, NGHBR)
                  GOTO 421
               ENDIF
            ENDIF
         ENDIF
  425 CONTINUE
C
      PEAKL(B) = W1
      PEAKR(B) = W2
      BASE(B)  = DCV
      BSTAR(B) = DCV
C
C *** STEP 5 ***  Return to SEARCH
C
  500 RETURN
      END
C***********************************************************************
      SUBROUTINE ADDMEM(MEMBER, NMEM, U)
C
C     KEEPS TRACK OF VERTICES MARKED L OR R DURING A CALL TO BLSAUG
C     IN CASE A NEW BLOSSOM IS FORMED
C
C-----------------------------------------------------------------------
      INTEGER MEMBER(1), NMEM, U
C-----------------------------------------------------------------------
      NMEM = NMEM + 1
      MEMBER(NMEM) = U
      RETURN
      END
C***********************************************************************
      SUBROUTINE FNDPTH(HIGH, LOW, B, JOB)
C
C     JOB .NE. -1:  RETURNS A LINKED LIST FROM HIGH TO LOW
C     JOB .EQ. -1:  RETURNS A LINKED LIST FROM LOW TO HIGH
C     JOB .EQ.  2:  ALLOWS TRANSFERS BETWEEN LEFT AND RIGHT SEARCHTREES
C                   (OCCURS ONLY WHEN OPENING A BLOSSOM)
C
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
C     PARAMETERS
C
      INTEGER N, M, NP1, ND8, MT2, ND2
      PARAMETER (N=8192, M=32768)
      PARAMETER (NP1=N+1, ND8=N/8, MT2=M*2, ND2=N/2)
C
C     ARGUMENTS 
C
      INTEGER HIGH, LOW, B, JOB
C
C     LOCAL VARIABLES 
C
      INTEGER ENTRNC, BASS, LB, RETADD
      INTEGER STACK(8, ND8), U, V, PREE, PNTR, SUCC, VINDEX
C
C     COMMON STORAGE
C
      INTEGER NI, MI, MATCH(N), CARD, NINDEX(NP1), NGHBR(MT2)
      COMMON NI, MI, MATCH, CARD, NINDEX, NGHBR
C
      CHARACTER*1 MARK(N)
      COMMON /BLK0/ MARK
C
      INTEGER EVLEV(N), ODLEV(N), BLOSS(N)
      COMMON /BLK1/ EVLEV, ODLEV, BLOSS
C
      LOGICAL VISITD(N)
      COMMON /BLK3/ VISITD
C
      INTEGER BASE(ND2), BSTAR(ND2), PEAKL(ND2), PEAKR(ND2),
     +        F(N), PATH(N)
      COMMON /BLK4/ BASE, BSTAR, PEAKL, PEAKR, F, PATH
C
      INTEGER PRED(2,M), PINDEX(N), PFREE, DERP(2,M), DINDEX(N), DFREE,
     +        PRDCTR(N)
      COMMON /BLK6/ PRED, PINDEX, PFREE, DERP, DINDEX, DFREE, PRDCTR
C
      INTEGER EDGE(MT2)
      COMMON /BLK7/ EDGE
C
      INTEGER STKLIM, STKTOP
      COMMON /BLK8/ STKLIM, STKTOP
C
      INTEGER LR(N), CALLS
      COMMON /BLK9/ LR, CALLS
C-----------------------------------------------------------------------
      IF (N .LT. NI) THEN
         WRITE(6, 4) 'N', N, NI
    4    FORMAT(1X, 'FNDPTH:  PARAMETER ', A1,' =',I10,
     +          ' IS TOO SMALL - INPUT VALUE =',I10)
         STOP
      ELSE IF (M .LT. MI) THEN
         WRITE(6, 4) 'M', M, MI
         STOP
      ENDIF
      STKLIM = ND8
      STKTOP = 0
C
C *** STEP 0 ***
C
   10 IF (HIGH .EQ. LOW) THEN
C
C        WHOLE PATH IS JUST "high"
C
         GOTO 820
      ENDIF
C
      V = HIGH
      VINDEX = PINDEX(V)
C
C *** STEP 1 ***
C
  100 IF (VINDEX .EQ. 0) THEN
         U = 0
      ELSE
         CALL ANCEST(V, 2, U, VINDEX, PRED, EDGE, MARK, NINDEX, NGHBR)
      ENDIF
      IF (U .EQ. 0) THEN
         IF (F(V) .EQ. 0) THEN
            PRINT *, 'ERROR --  FNDPTH CANNOT FIND PATH'
            STOP
         ELSE 
            V = F(V)
         ENDIF
         VINDEX = PINDEX(V)
         GOTO 100
      ENDIF
C
C *** STEP 2 ***
C
      IF (BLOSS(V) .EQ.  B) THEN
C
C        MARK U-V 'VISITED'
C
         CALL STATUS(U, V, EDGE, 2, 'W', NINDEX, NGHBR)
      ELSE
         U = BASE(BLOSS(V))
      ENDIF
C
C *** STEP 3 ***
C
      IF (U .EQ. LOW) GOTO 600
C
C *** STEP 4 ***  
C
      IF  (VISITD(U)) GOTO 100
      IF ( MIN(EVLEV(U),ODLEV(U)) .LE. MIN(EVLEV(LOW),ODLEV(LOW)) ) 
     +   GOTO 100
      IF (JOB .EQ. 2) GOTO 501
      IF ((BLOSS(U) .EQ. B) .AND. (LR(U) .EQ. -LR(HIGH))) GOTO 100 
C *** STEP 5 ***
C
  501 VISITD(U) = .TRUE.
      F(U) = V
      V = U
      VINDEX = PINDEX(V)
      GOTO 100
C
C *** STEP 6 ***
C
  600 PATH(V) = LOW
  620 IF (V .NE. HIGH) THEN
         U = V
         V = F(V)
         PATH(V) = U
         GOTO 620
      ENDIF
C
C *** STEP 7 ***
C
      ENTRNC = HIGH
  710 IF (ENTRNC .NE. LOW) THEN
         BASS = PATH(ENTRNC)
         IF (BLOSS(ENTRNC) .NE. B) THEN
C
C            SIMULATE CALL TO OPEN(ENTRNC, BASS)
C
             GOTO 900
C
         ENDIF
  777    ENTRNC = BASS
         GOTO 710
      ENDIF
C
C *** STEP 8 ***
C
  800 IF (JOB .EQ. -1) THEN
C
C     INVERT PATH IF NECESSARY
C
      PREE = 0
      PNTR = HIGH
      SUCC = PATH(HIGH)
  810 IF (PNTR .NE. LOW) THEN
         PATH(PNTR) = PREE
         PREE = PNTR
         PNTR = SUCC
         SUCC = PATH(PNTR)
         GOTO 810
      ENDIF
      PATH(PNTR) = PREE
      ENDIF
C
C     CHECK STACK BEFORE RETURNING
C
  820 IF (STKTOP .NE. 0) THEN
         HIGH   = STACK(1, STKTOP)
         LOW    = STACK(2, STKTOP)
         B      = STACK(3, STKTOP)
         JOB    = STACK(4, STKTOP)
         ENTRNC = STACK(5, STKTOP)
         BASS   = STACK(6, STKTOP)
         LB     = STACK(7, STKTOP)
         RETADD = STACK(8, STKTOP)
         STKTOP = STKTOP - 1
      ELSE
         RETURN
      ENDIF
C
      IF (RETADD .EQ. 777) THEN
         GOTO 777
      ELSE IF (RETADD .EQ. 902) THEN
         GOTO 902
      ELSE
         GOTO 904
      ENDIF
C
C***********************************************************************
C
C     FUNCTION OPEN STARTS AT LINE 900
C     EMBEDDED INSIDE FNDPTH TO REMOVE RECURSION
C
  900 LB = BLOSS(ENTRNC)
      IF (EVLEV(ENTRNC) .LE. ODLEV(ENTRNC) ) THEN
         CALL PUSH(HIGH, LOW, B, JOB, ENTRNC, BASS, LB, 777, STACK)
         HIGH = ENTRNC
         LOW = BASS
         JOB = 2
         B = LB
         GOTO 10
      ELSE
         IF (LR(ENTRNC) .GT. 0) THEN
            CALL PUSH(HIGH, LOW, B, JOB, ENTRNC, BASS, LB, 902, STACK)
            HIGH = PEAKL(LB)
            LOW = ENTRNC
            JOB = -1
            B = LB
            GOTO 10
  902    PATH(PEAKL(LB)) = PEAKR(LB)
            CALL PUSH(HIGH, LOW, B, JOB, ENTRNC, BASS, LB, 777, STACK)
            HIGH= PEAKR(LB)
            LOW = BASS
            JOB = 1
            B = LB
            GOTO 10
         ELSE
            CALL PUSH(HIGH, LOW, B, JOB, ENTRNC, BASS, LB, 904, STACK)
            HIGH = PEAKR(LB)
            LOW = ENTRNC
            JOB = -1
            B = LB
            GOTO 10
  904    PATH(PEAKR(LB)) = PEAKL(LB)
            CALL PUSH(HIGH, LOW, B, JOB, ENTRNC, BASS, LB, 777, STACK)
            HIGH = PEAKL(LB)
            LOW = BASS
            JOB = 1
            B = LB
            GOTO 10
         ENDIF
      ENDIF
      END
C***********************************************************************
      SUBROUTINE ANCEST(V, JOB, U, INDEX, PRED, EDGE, MARK,NINDEX,NGHBR)
C
C     SEARCHES PREDECESSOR LIST OF V
C         JOB = 1:  CHECK FOR UNUSED EDGES
C         JOB = 2:  CHECK FOR UNVISITED EDGES
C
C         U .EQ. 0:  NO SUITABLE EDGE WAS FOUND
C         U .NE. 0:  U-V IS A SUITABLE EDGE
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
      INTEGER V, JOB, U, INDEX, PRED(2,*), EDGE(*), NINDEX(*), NGHBR(*)
      CHARACTER*1 MARK(*)
C
      INTEGER W, CODE, K
C-----------------------------------------------------------------------
      U = 0
  201 IF ((INDEX .NE. 0) .AND. (U .EQ. 0)) THEN
         W = PRED(2, INDEX)
         INDEX = PRED(1, INDEX)
         IF (MARK(W) .NE. 'E') THEN
            CALL STATUS(W, V, EDGE, CODE, 'R', NINDEX, NGHBR)
            IF (JOB .EQ. 1) THEN
               IF (MOD(CODE, 2) .EQ. 0) U = W
            ELSE
               IF (CODE .LT. 2) U = W
            ENDIF
         ENDIF
         GOTO 201
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE STATUS(U, V, EDGE, CODE, JOB, NINDEX, NGHBR)
C
C     JOB .EQ. 'W' (WRITE):, ADD CODE TO STATUS(U,V)
C     JOB .EQ. 'R' (READ) :  SETS CODE =  STATUS(U,V)
C     STATUS(U,V) IS SYMMETRIC SO UPPER TRIANGLE IS STORED IN EDGE
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
      INTEGER U, V, EDGE(*), CODE, NINDEX(*), NGHBR(*)
      CHARACTER JOB
C
      INTEGER I, J, K, INDEX
C-----------------------------------------------------------------------
      IF (U.LT.V) THEN
          I = U
          J = V
      ELSE
          I = V
          J = U
      ENDIF
      INDEX = 0
      DO 10 K=NINDEX(I), NINDEX(I+1)-1
         IF (NGHBR(K) .EQ. J) INDEX = K
   10 CONTINUE
      IF (INDEX .EQ. 0) THEN
         PRINT *, 'STATUS:  CANNOT FIND EDGE ', I, J
         STOP
      ENDIF
      IF (JOB .EQ. 'W') THEN
         EDGE(INDEX) = EDGE(INDEX) + CODE
      ELSE
         CODE = EDGE(INDEX)
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE ADDBRG(U, V, BR, BRIDGE, BINDEX, BFREE)
C
C     ADDS EDGE (U,V) TO BRIDGE(BR)
C
C     U, V   -  INTEGER VERTICIES
C     BR     -  INTEGER BRIDGE LEVEL NUMBER
C     BRIDGE -  ARRAY OF SIZE 3 x M  HOLDING ALL BRIDGES
C     BINDEX -  ARRAY OF SIZE N/2 HOLDING POINTERS TO EACH LEVEL
C     BFREE  -  INTEGER POINTING TO FIRST FREE LOCATION IN BRIDGE
C     NEXT   -  INTEGER POINTER
C
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
      INTEGER U, V, BR, BRIDGE(3,*), BINDEX(*), BFREE
      INTEGER NEXT
C-----------------------------------------------------------------------
      IF (BFREE .EQ. 0) THEN
         WRITE(6,20) BR, U, V
   20    FORMAT(1X, 'ADDBRG:  STACK OVERFLOW ERROR',
     +          5X, 'BR, U, V:',3I10)
         STOP
      ELSE
         NEXT = BFREE
         BFREE = BRIDGE(1, NEXT)
         BRIDGE(2, NEXT) = U
         BRIDGE(3, NEXT) = V
         BRIDGE(1, NEXT) = BINDEX(BR)
         BINDEX(BR)       = NEXT
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE ADDSTK(STACK, INDEX, FREE, U, V)
C
C     ADDS ELEMENT V TO SUBSTACK U
C
C     STACK  -  ARRAY OF SIZE 2 x M  HOLDING ALL SUBSTACKS
C     INDEX  -  ARRAY OF SIZE N HOLDING POINTERS TO EACH SUBSTACK
C     FREE   -  INTEGER POINTING TO FIRST FREE LOCATION IN STACK
C     U      -  INTEGER VERTEX
C     V      -  INTEGER VERTEX
C     NEXT   -  INTEGER POINTER
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
      INTEGER STACK(2,*), INDEX(*), FREE, U, V
      INTEGER NEXT
C-----------------------------------------------------------------------
      IF (FREE .EQ. 0) THEN
         WRITE(6,20) U, V
   20    FORMAT(1X, 'ADDSTK:  STACK OVERFLOW ERROR',
     +          5X, 'U, V:',2I10)
         STOP
      ELSE
         NEXT = FREE
         FREE = STACK(1, NEXT)
         STACK(2, NEXT) = V
         STACK(1, NEXT) = INDEX(U)
         INDEX(U)       = NEXT
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE PUSH(HI, LO, B, JOB, ENT, BAS, LB, RA, STACK)
C
C     ADDS ELEMENTS TO STACK USED TO SIMULATE RECURSIVE CALLS
C     BETWEEN FNDPTH AND OPEN
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
C
      INTEGER HI, LO, B, JOB, ENT, BAS, LB, RA, STACK(8,*)
C
      INTEGER STKLIM, STKTOP
      COMMON /BLK8/ STKLIM, STKTOP
C-----------------------------------------------------------------------
      IF (STKTOP .LT. STKLIM) THEN
          STKTOP = STKTOP + 1
          STACK(1, STKTOP) = HI
          STACK(2, STKTOP) = LO
          STACK(3, STKTOP) = B
          STACK(4, STKTOP) = JOB
          STACK(5, STKTOP) = ENT
          STACK(6, STKTOP) = BAS
          STACK(7, STKTOP) = LB
          STACK(8, STKTOP) = RA
      ELSE
         WRITE(6, 9) HI, LO, B, JOB, RA
    9    FORMAT(1X, 'PUSH:  STACK OVERFLOW ERROR',
     +          5X, 'HI, LO, B, JOB, RA =', 5I10)
         STOP
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE BASTAR(V, U, BSTAR, F, BLOSS)
C
C     SETS U = BASE*(U) AND PRESERVES PATH BACK TO OLDV
C-----------------------------------------------------------------------
      IMPLICIT CHARACTER (A-Z)
      INTEGER V, U, BSTAR(*), F(*), BLOSS(*)
      INTEGER OLDV, W
C-----------------------------------------------------------------------
      OLDV = V
   10 F(U) = V
      V = U
      U = BSTAR(BLOSS(U))
      IF (BLOSS(U) .NE. 0) GOTO 10
C
      W = F(V)
      IF (OLDV .EQ. 0) THEN
         F(U) = V
         V = 0
      ENDIF
C
C     PATH COMPRESSION
C
   20 IF (W .NE. OLDV) THEN
         BSTAR(BLOSS(W)) = U
         W = F(W)
         GOTO 20
      ENDIF
      RETURN
      END
C***********************************************************************
      SUBROUTINE SSORT(A,B,L)
C
C     *      SORTING OF THE VECTOR A(I) IN INCREASING ORDER BY THE     *
C     *      SHELL-ALGORITHM.                                          *
C     *                                                                *
C     *      PARAMETERS:                                               *
C     *      INPUT:                                                    *
C     *         L        DIMENSION OF THE VECTOR                       *
C     *         A(I)     VECTOR TO SORT (INTEGER)                      *
C     *         B(I)     = I    (INTEGER)  I=1,...,N                   *
C     *      OUTPUT:                                                   *
C     *         A(I)     THE SORTED VECTOR                             *
C     *         B(I)     PERMUTATION VECTOR OF THE SORTED VECTOR       *
C     *                                                                *
C *** ******************************************************************
C - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      IMPLICIT INTEGER (A-Z)
      DIMENSION A(1), B(1)
C - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      F = 1
      IF (L.LE.F) RETURN
      N2 = (L - F+1)/2
      S = 1023
      DO 100 T = 1, 10
      IF (S.GT.N2) GOTO 90
      LS = L - S
      DO 20 I = F, LS
      IS = I + S
      AH = A(IS)
      BH = B(IS)
      J = I
      JS = IS
    5 IF (AH.GE.A(J)) GOTO 10
      A(JS) = A(J)
      B(JS) = B(J)
      JS = J
      J = J - S
      IF (J.GE.F) GOTO 5
   10 A(JS) = AH
      B(JS) = BH
   20 CONTINUE
   90 S = S/2
  100 CONTINUE
      RETURN
      END
