% BASE PROGRAM

% Check feature requirements

:- requires(feature(actionCosts)).
:- requires(feature(axiomRules)).
:- requires(feature(conditionalEffects)).

% Projection

postcondition(A,X,V) :- postcondition(A,E,X,V).

has_condition(A,X,0) :- action(A), precondition(A,X,V).
has_condition(A,X,1) :- action(A), postcondition(A,X,V).

% Define active/1, fluent/2 and fluent/1
active(A)   :- action(A).
fluent(X,V) :- contains(X,V).
fluent(X)   :- fluent(X,V).

