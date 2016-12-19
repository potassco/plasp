% Constant '_parallel' to enable parallel actions
% - value '1': "forall" parallel actions that can be arranged in any sequence
% - value '2': "exists" parallel actions that can be arranged in some sequence
% - otherwise: sequential actions

#const _parallel = 0.

%#include <incmode>.

% BASE PROGRAM

% Define auxiliary predicates for actions w.r.t. parallel mode

diverge(A1,A2,X) :- active(A1), active(A2), A1 < A2, postcondition(A1,X,V),
                    has_condition(A2,X,1), not postcondition(A2,X,V),
                    _parallel = 1 : _parallel != 2.
diverge(A1,A2)   :- diverge(A1,A2,X).

exclude(A1,A2) :- diverge(A1,A2), precondition(A1,X,V), _parallel = 1,
                  has_condition(A2,X,0), not precondition(A2,X,V).

disable(A1,A2) :- active(A1), active(A2), A1 != A2, postcondition(A1,X,V),
                  has_condition(A2,X,0), not precondition(A2,X,V),
                  _parallel = 2, not diverge(A1,A2), not diverge(A2,A1).

scope(X,V) :- active(A), precondition(A,X,V), _parallel = 2.

% Define relevant fluents w.r.t. parallel mode

fluent(X,V) :- produce(X,V).
fluent(X,V) :- persist(X,V).
fluent(X,V) :- initialState(X,V), fluent(X).
fluent(X,V) :- active(A), postcondition(A,X,V), fluent(X).
fluent(X)   :- fluent(X,V).
fluent(X)   :- diverge(A1,A2,X), not exclude(A1,A2).

% Define unsubsumed mutexes

mutex(G,X) :- mutexGroup(G), contains(G,X,V), fluent(X,V).
mutex(G)   :- mutexGroup(G), #count{X : mutex(G,X)} > 1.

% Define initial state

holds(X,V,0) :- initialState(X,V), fluent(X).

:- fluent(X), #count{V : holds(X,V,0)} > 1.
:- mutex(G), #count{X,V : holds(X,V,0), contains(G,X,V)} > 1.

% STEP PROGRAM

#program step(t).

% Generate successor state

1 {holds(X,V,t) : fluent(X,V)} 1 :- fluent(X).

:- mutex(G), #count{X,V : holds(X,V,t), contains(G,X,V)} > 1.

change(X,t) :- holds(X,V,t-1), not holds(X,V,t).

% Generate actions

0 {occurs(A,t) : active(A)}.

:- occurs(A,t), postcondition(A,X,V), fluent(X), not holds(X,V,t).

effect(X,t) :- occurs(A,t), postcondition(A,X,V), fluent(X), not precondition(A,X,V).

:- change(X,t), not effect(X,t).

% Checks w.r.t. parallel mode

:- _parallel != 1, _parallel != 2, #count{A : occurs(A,t)} > 1.

:- _parallel != 2, occurs(A,t), precondition(A,X,V), not holds(X,V,t-1).

:- _parallel = 1, occurs(A,t), precondition(A,X,V), not has_condition(A,X,1), not holds(X,V,t).

single(X,t) :- occurs(A,t), precondition(A,X,V), _parallel = 1,
               has_condition(A,X,1), not postcondition(A,X,V).

:- single(X,t), #count{A : occurs(A,t), postcondition(A,X,V), not precondition(A,X,V)} > 1.

proceed(X,V,t) :- holds(X,V,t-1), scope(X,V).
proceed(X,V,t) :- occurs(A,t), postcondition(A,X,V), scope(X,V), not precondition(A,X,V),
                  perform(A,t).

perform(A1,t) :- active(A1), _parallel = 2, not occurs(A1,t).
perform(A1,t) :- active(A1), _parallel = 2,
                 proceed(X,V,t) : precondition(A1,X,V); perform(A2,t) : disable(A1,A2).

:- _parallel = 2, active(A), not perform(A,t).

% CHECK PROGRAM

#program check(t).

% Check goal conditions

:- query(t), goal(X,V), not holds(X,V,t).

% DISPLAY PART

#show occurs/2.
