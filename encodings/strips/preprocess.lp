% Constant '_closure' to (de)activate analysis of potentially relevant actions
% - value '1': forward chaining of effects w.r.t. initial variable values
% - value '2': backward regression of effects w.r.t. goal variable values
% - value '3': both forward chaining and backward regression of effects
% - otherwise: off

#const _closure = 3.

% BASE PROGRAM

% Check feature requirements

:- requires(feature(actionCosts)).
:- requires(feature(axiomRules)).
:- requires(feature(conditionalEffects)).

% Basic redundancy check for actions

postcondition(A,X,V) :- postcondition(A,E,X,V).

has_condition(A,X,0) :- action(A), precondition(A,X,V).
has_condition(A,X,1) :- action(A), postcondition(A,X,V).

inconsistent(A) :- has_condition(A,X,P),
                   #count{V : precondition(A,X,V), P = 0;
                          V : postcondition(A,X,V), P = 1} > 1.
consistent(A)   :- action(A), not inconsistent(A).
irredundant(A)  :- consistent(A), postcondition(A,X,V), not precondition(A,X,V).

% Forward chaining of effects w.r.t. initial variable values

feasible(X,V) :- initialState(X,V).
feasible(X,V) :- possible(A), postcondition(A,X,V).

possible(A) :- irredundant(A), feasible(X,V) : precondition(A,X,V).
possible(A) :- irredundant(A), _closure != 1, _closure != 3.

:- goal(X,V), not feasible(X,V).

% Backward regression of effects w.r.t. goal variable values

produce(X,V) :- goal(X,V), not initialState(X,V).
produce(X,V) :- active(A), precondition(A,X,V), not initialState(X,V).
produce(X,V) :- persist(X,V), active(A), has_condition(A,X,1), not postcondition(A,X,V).

persist(X,V) :- goal(X,V), initialState(X,V).
persist(X,V) :- active(A), precondition(A,X,V), initialState(X,V).

active(A) :- possible(A), postcondition(A,X,V), produce(X,V).
active(A) :- possible(A), _closure != 2, _closure != 3.
