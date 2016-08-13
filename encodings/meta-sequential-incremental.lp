#include <incmode>.

% Check feature requirements
:- requiresFeature(actionCosts).
:- requiresFeature(axiomRules).
:- requiresFeature(conditionalEffects).

#program base.

% Establish initial state
holds(Variable, Value, 0) :- initialState(Variable, Value).

% Make unspecified initial state variables false by default (for PDDL)
holds(variable(Variable), value(Variable, false), 0) :- variable(variable(Variable)), {initialState(variable(Variable), _)} 0.

#program step(t).

% Perform actions
1 {occurs(Action, t) : action(Action)} 1.

% Check preconditions
:- occurs(Action, t), precondition(Action, Variable, Value), not holds(Variable, Value, t - 1).

% Apply effects
caused(Variable, Value, t) :- occurs(Action, t), postcondition(Action, _, Variable, Value).
modified(Variable, t) :- caused(Variable, Value, t).

holds(Variable, Value, t) :- caused(Variable, Value, t).
holds(Variable, Value, t) :- holds(Variable, Value, t - 1), not modified(Variable, t).

% Check that variables have unique values
:- variable(Variable), not 1 {holds(Variable, Value, t) : contains(Variable, Value)} 1.

% Check mutexes
:- mutexGroup(MutexGroup), not {holds(Variable, Value, t) : contains(MutexGroup, Variable, Value)} 1.

#program check(t).

% Verify that goal is met
:- query(t), goal(Variable, Value), not holds(Variable, Value, t).

#show query/1.
#show occurs/2.
