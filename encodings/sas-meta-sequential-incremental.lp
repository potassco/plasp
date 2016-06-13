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
caused(Var, Val, t) :- occurs(A, t), postcondition(A, _, Var, Val).
modified(Var, t) :- caused(Var, Val, t).

holds(Var, Val, t) :- caused(Var, Val, t).
holds(Var, Val, t) :- holds(Var, Val, t - 1), not modified(Var, t).

% Check that variables have unique values
:- variable(X), Var = variable(X), not 1 {holds(Var, Val, t) : contains(Var, Val)} 1.

% Check mutexes
:- mutexGroup(X), M = mutexGroup(X), not {holds(Var, Val, t) : contains(M, Var, Val)} 1.

#program check(t).

% Verify that goal is met
:- query(t), goal(Var, Val), not holds(Var, Val, t).

#show query/1.
#show occurs/2.
