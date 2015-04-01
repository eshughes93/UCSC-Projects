/******************
Evan Hughes
CMPS112 Homework 3
Prolog
Due Feb 4, 2015
*****************/
% (7)
father(al, bud).
father(al, kelly).
mother(peggy, kelly).
mother(peggy, bud).
mother(martha, peggy).

% (8)
grandma(martha,kelly).
grandma(martha,bud).

% (9)
descendants(X,Y):- 
    father(X,Y);
    mother(X,Y).
descendants(X,Y):-
    father(X,Z),
    descendants(Z,Y).
descendants(X,Y):-
    mother(X,Z),
    descendants(Z,Y).
        

% (10)
siblings(X,Y):-
    father(Z,X), 
    father(Z,Y), 
    mother(W,X), 
    mother(W,Y), 
    not(X=Y).

% (11)
%    ASCII-ART for the NFA:
%
%    (q0)  ---a-->  (q1)  ---b-->  (q2*)
%     |
%     a
%     |
%     V  / --<-- \
%    (q3*)        a
%        \ -->-- /

%  Transition relation:
transition(q0,q1,a).
transition(q1,q2,b).
transition(q0,q3,a).
transition(q3,q3,a).

%  Accepting states:
accepting(q2).
accepting(q3).

accepts(State,InputList):-
   InputList = [],
   accepting(State).
accepts(State,InputList):-
   InputList = [H|T],
   transition(State, NewState, H),
   accepts(NewState,T).
