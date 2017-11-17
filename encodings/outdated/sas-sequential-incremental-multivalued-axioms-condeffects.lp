#include <incmode>.

% Check feature requirements
% :- requires(feature(actionCosts)).

#program base.

% Establish initial state
holds(Variable, Value, 0) :- initialState(Variable, Value), not stateVariable(Variable).

% Identify (head and) state variables
headVariable(Rule,Variable) :- variable(Variable), axiomRule(Rule), postcondition(Rule,_,Variable,_).
stateVariable(Variable)     :- headVariable(_,Variable).

#program step(t). % t=1,2,...

% Perform actions
1 {occurs(Action, t) : action(Action)} 1.

% Check preconditions
:- occurs(Action, t), precondition(Action, Variable, Value), not holds(Variable, Value, t - 1).

% Apply effects
caused(Variable, Value, t) :- occurs(Action, t), postcondition(Action, Effect, Variable, Value),
			      holds(Variable', Value', t - 1) : precondition(Effect, Variable', Value').
changed(Variable, t) :- caused(Variable, Value, t).

holds(Variable, Value, t) :- caused(Variable, Value, t).
holds(Variable, Value, t) :- holds(Variable, Value, t - 1), not changed(Variable, t), not stateVariable(Variable).

#program check(t). % t=0,1,...

% Check that variables have unique values
:- variable(Variable), {holds(Variable, Value, t) : contains(Variable, Value)} != 1.

% Check mutexes
:- mutexGroup(MutexGroup),  {holds(Variable, Value, t) : contains(MutexGroup, Variable, Value)} > 1.

% Apply axioms
derived(Variable, Value, t) :- axiomRule(Rule), postcondition(Rule,_,Variable,Value),
			       holds(Variable', Value', t) : precondition(Rule, Variable', Value'), not headVariable(Rule,Variable').

modified(Variable, t) :- derived(Variable, Value, t).

holds(Variable, Value, t) :-      derived(Variable, Value, t), stateVariable(Variable).
holds(Variable, Value, t) :- not modified(Variable,        t), stateVariable(Variable), initialState(Variable, Value).

% Verify that goal is met
:- query(t), goal(Variable, Value), not holds(Variable, Value, t).

#show occurs/2.
% #show holds/3.
