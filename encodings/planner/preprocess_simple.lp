% BASE PROGRAM

% Check feature requirements

:- requires(feature(actionCosts)).
:- requires(feature(axiomRules)).
:- requires(feature(conditionalEffects)).

% Basic redundancy check for actions

postcondition(A,X,V) :- postcondition(A,E,X,V).

has_condition(A,X,0) :- action(A), precondition(A,X,V).
has_condition(A,X,1) :- action(A), postcondition(A,X,V).

active(A) :- action(A).
fluent(X) :- active(A), has_condition(A,X,1).
