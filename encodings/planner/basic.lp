#program base. % as strips-incremental.lp

% Define unsubsumed mutexes

mutex(G,X) :- mutexGroup(G), contains(G,X,V). %, fluent(X,V).
mutex(G)   :- mutexGroup(G), #count{X : mutex(G,X)} > 1.

% Define initial state

holds(X,V,0) :- initialState(X,V). %, fluent(X).

#program step(t). % new

% generate (a SAT problem)
{     holds(X,V,t) } :- contains(X,V).
{ occurs(A,t) : action(A) }.

% effects
:- occurs(A,t), postcondition(A,_,X,V), not holds(X,V,t).

% preconditions
:- occurs(A,t), precondition(A,X,V),    not holds(X,V,t-1).

% inertia
:- holds(X,V,t), not holds(X,V,t-1), 
   not occurs(A,t) : postcondition(A,_,X,V).

% existence and uniqueness of value
:- not 1 { holds(X,V,t) } 1, variable(X). 

% mutexes
:- 2 #sum { 1,X,V: holds(X,V,t), contains(G,X,V) }, mutex(G).

#program check(t). % as strips-incremental.lp

% Check goal conditions

:- query(t), goal(X,V), not holds(X,V,t).
#show occurs/2.
