#include <incmode>.

#program base.

% Establish initial state
holds(Var, Val, 0) :- initialState(Var, Val).

#program step(t).

% Perform actions
1 {occurs(action(A), t) : action(A)} 1.

% Check preconditions
:- occurs(A, t), precondition(A, Var, Val), not holds(Var, Val, t - 1).

% Apply effects
caused(Var, Val, t) :- occurs(A, t), postcondition(A, Var, Val).
modified(Var, t) :- caused(Var, Val, t).

holds(Var, Val, t) :- caused(Var, Val, t).
holds(Var, Val, t) :- holds(Var, Val, t - 1), not modified(Var, t).

#program check(t).

% Verify that goal is met
:- query(t), goal(Var, Val), not holds(Var, Val, t).

#show query/1.
#show occurs/2.
