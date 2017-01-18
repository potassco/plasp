#const horizon=1.

% Check feature requirements
:- requires(feature(actionCosts)).
:- requires(feature(axiomRules)).
:- requires(feature(conditionalEffects)).

% Horizon, must be defined externally
time(0..horizon).

% Establish initial state
holds(Variable, Value, 0) :- initialState(Variable, Value).

% Perform actions
1 {occurs(Action, T) : action(Action)} 1 :- time(T), T > 0.

% Check preconditions
:- occurs(Action, T), precondition(Action, Variable, Value), not holds(Variable, Value, T - 1), time(T), time(T - 1).

% Apply effects
caused(Variable, Value, T) :- occurs(Action, T), postcondition(Action, _, Variable, Value).
modified(Variable, T) :- caused(Variable, Value, T).

holds(Variable, Value, T) :- caused(Variable, Value, T), time(T).
holds(Variable, Value, T) :- holds(Variable, Value, T - 1), not modified(Variable, T), time(T), time(T - 1).

% Check that variables have unique values
:- variable(Variable), not 1 {holds(Variable, Value, T) : contains(Variable, Value)} 1, time(T).

% Check mutexes
:- mutexGroup(MutexGroup), not {holds(Variable, Value, T) : contains(MutexGroup, Variable, Value)} 1, time(T).

% Verify that goal is met
:- goal(Variable, Value), not holds(Variable, Value, horizon).

#show occurs/2.
