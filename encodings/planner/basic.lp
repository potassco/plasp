% Constant '_parallel' to enable parallel actions
% - value '0': parallel actions *without* checking for mutual disablement
% - value '1': "forall" parallel actions that can be arranged in any sequence
% - value '3': "exists" parallel actions omitting achievement of preconditions
% - value '5': sequential actions

#const _parallel = 0.

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


%
% Parallel planning settings
%

#program base.

% _parallel = 1
selfdefeat(A,X) :- action(A), precondition(A,X,V),
                       postcondition(A,X,_),
                   not postcondition(A,X,V), _parallel = 1.

% _parallel = 3
disable(A1,A2) :- action(A1), action(A2), A1 != A2,
                  postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.

#program step(t).

% _parallel = 1
single(X,T) :- occurs(A,T), selfdefeat(A,X), _parallel = 1.
:- single(X,T),                              _parallel = 1,
   #count{A : occurs(A,T),
                  postcondition(A,X,V),
              not precondition(A,X,V)} > 1.

% _parallel = 3
perform(A,T) :- action(A),                  _parallel = 3,
                alright(A1,T) : disable(A,A1).

alright(A,T) :- action(A), not occurs(A,T), _parallel = 3.
alright(A,T) :- perform(A,T),               _parallel = 3.

:- action(A), not alright(A,T),             _parallel = 3.

% _parallel = 5
:- 2 { occurs(A,t) },                       _parallel = 5.
