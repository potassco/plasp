#include <incmode>.

#program base.

% Establish initial state
holds(F, 0) :- initialState(F).

#program step(t).

% Perform actions
1 {occurs(A, t) : action(A)} 1.

% Check preconditions
:- occurs(A, t), precondition(A, F, true), not holds(F, t - 1).
:- occurs(A, t), precondition(A, F, false), holds(F, t - 1).

% Apply effects
holds(F, t) :- occurs(A, t), postcondition(A, F, true), action(A).
deleted(F, t) :- occurs(A, t), postcondition(A, F, false), action(A).
holds(F, t) :- holds(F, t - 1), not deleted(F, t).

#program check(t).

% Verify that goal is met
:- query(t), goal(F, true), not holds(F, t).
:- query(t), goal(F, false), holds(F, t).

#show query/1.
#show holds/2.
#show occurs/2.
