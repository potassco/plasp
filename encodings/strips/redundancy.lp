% Additional rules for enforcing the inclusion of parallel actions in plans,
% whenever such 'redundant' actions are compatible with states and other actions

% BASE PROGRAM

selfdefeat(A) :- selfdefeat(A,X).
disjoin(A,A1) :- active(A), has_condition(A,X,P), selfdefeat(A1,X), not selfdefeat(A).

compatible(A,A1) :- active(A), active(A1), A != A1, 1 < _parallel, _parallel < 4,
                    not diverge(A,A1), not exclude(A,A1), not disable(A,A1) : disable(A1,A).
compatible(A)    :- compatible(A,A1).
compatible(A)    :- active(A), active(A1), A != A1, _parallel = 1, not selfdefeat(A),
                    not diverge(A,A1), not exclude(A,A1), not disjoin(A,A1).

disable(A,A1,A2) :- compatible(A,A1), compatible(A,A2), compatible(A1,A2),
                    disable(A1,A2), not disable(A1,A).

disabled(A,A1) :- compatible(A), disable(A,A1).
disabled(A,A2) :- simulate(A,A1), disable(A,A1,A2).

simulate(A,A1) :- compatible(A,A1), not disable(A1,A), disabled(A,A1).
simulate(A,A1) :- compatible(A,A1), not disable(A1,A), scope(X,V),
                  postcondition(A1,X,V), not precondition(A1,X,V).

% STEP PROGRAM

#program step(t).

defeated(A,t) :- compatible(A), postcondition(A,X,V), fluent(X), not holds(X,V,t).

defeated(A,t) :- compatible(A), precondition(A,X,V), not holds(X,V,t-1), _parallel != 2.
defeated(A,t) :- compatible(A), precondition(A,X,V), not has_condition(A,X,1), not holds(X,V,t),
                 _parallel = 1.
defeated(A,t) :- compatible(A), has_condition(A,X,1), not has_condition(A,X,0), single(X,t).

proceed(A,X,V,t) :- compatible(A), holds(X,V,t-1), scope(X,V).
proceed(A,X,V,t) :- simulate(A,A1), perform(A,A1,t), occurs(A1,t), scope(X,V),
                    postcondition(A1,X,V), not precondition(A1,X,V).

perform(A,A1,t) :- simulate(A,A1),
                   proceed(A,X,V,t) : precondition(A1,X,V), _parallel = 2;
                   alright(A,A2,t) : disable(A,A1,A2).

alright(A,A1,t) :- disabled(A,A1), not occurs(A1,t).
alright(A,A1,t) :- disabled(A,A1), perform(A,A1,t).

defeated(A,t) :- compatible(A), precondition(A,X,V), not proceed(A,X,V,t), _parallel = 2.
defeated(A,t) :- compatible(A), disable(A,A1), not alright(A,A1,t).

:- compatible(A), not occurs(A,t), not defeated(A,t), 0 < _parallel, _parallel < 4.
