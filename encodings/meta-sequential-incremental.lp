#include <incmode>.

% Check feature requirements
:- requiresFeature(actionCosts).
:- requiresFeature(axiomRules).
:- requiresFeature(conditionalEffects).

#program base.

% Establish initial state
holds(Var, Val, 0) :- initialState(Var, Val).

#program step(t).

% Perform actions
1 {occurs(action(A), t) : action(A)} 1.

% Check preconditions
:- occurs(A, t), precondition(A, Var, Val), not holds(Var, Val, t - 1).

% Apply effects
caused(Var, Val, t) :- occurs(A, t), contains(A, E), postcondition(E, Var, Val).
modified(Var, t) :- caused(Var, Val, t).

holds(Var, Val, t) :- caused(Var, Val, t).
holds(Var, Val, t) :- holds(Var, Val, t - 1), not modified(Var, t).

% Check that variables without <none of those> values are unique
:- variable(X), Var = variable(X), not contains(Var, noneValue), not 1 {holds(Var, Val, t) : contains(Var, Val)} 1.
:- contains(Var, noneValue), not {holds(Var, Val, t) : contains(Var, Val)} 1.

% Check mutexes
:- mutexGroup(X), M = mutexGroup(X), not {holds(Var, Val, t) : contains(M, Var, Val)} 1.

% Output predicate
do(L, t) :- occurs(A, t), label(A, action(L)).

#program check(t).

% Verify that goal is met
:- query(t), goal(Var, Val), not holds(Var, Val, t).

#show query/1.
#show do/2.
