#include <incmode>.

#program base.

% Establish initial state
holds(Predicate, 0) :- initialState(Predicate).

#program step(t).

% Perform actions
1 {occurs(action(Action), t) : action(Action)} 1.

% Check preconditions
:- occurs(Action, t), precondition(Action, Predicate, true), not holds(Predicate, t - 1).
:- occurs(Action, t), precondition(Action, Predicate, false), holds(Predicate, t - 1).

% Apply effects
caused(Predicate, true, t) :- occurs(Action, t), postcondition(Action, Predicate, true).
caused(Predicate, false, t) :- occurs(Action, t), postcondition(Action, Predicate, false).

holds(Predicate, t) :- caused(Predicate, true, t).
holds(Predicate, t) :- holds(Predicate, t - 1), not caused(Predicate, false, t).

#program check(t).

% Verify that goal is met
:- query(t), goal(Predicate, true), not holds(Predicate, t).
:- query(t), goal(Predicate, false), holds(Predicate, t).

#show query/1.
#show occurs/2.
