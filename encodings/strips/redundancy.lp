% Additional rules for enforcing the inclusion of parallel actions in plans,
% whenever such 'redundant' actions are compatible with states and other actions

#program step(t).

defeated(A,t) :- active(A), postcondition(A,X,V), fluent(X), not holds(X,V,t),
                 _parallel = 1 : _parallel != 2.

defeated(A,t) :- _parallel = 1, active(A), precondition(A,X,V), not holds(X,V,t-1).
defeated(A,t) :- _parallel = 1, active(A), precondition(A,X,V), not holds(X,V,t).
defeated(A,t) :- _parallel = 1, active(A), postcondition(A,X,V), not precondition(A,X,V),
                 single(X,t).

proceed(A,X,V,t) :- active(A), holds(X,V,t-1), scope(X,V).
proceed(A,X,V,t) :- active(A), occurs(A1,t), perform(A,A1,t), A != A1,
                    postcondition(A1,X,V), scope(X,V), not precondition(A1,X,V).

perform(A,A1,t) :- active(A), active(A1), A != A1, _parallel = 2, not occurs(A1,t).
perform(A,A1,t) :- active(A), active(A1), _parallel = 2,
                   proceed(A,X,V,t) : precondition(A1,X,V); perform(A,A2,t) : disable(A1,A2).

defeated(A,t) :- _parallel = 2, active(A), not perform(A,A,t).

:- active(A), not occurs(A,t), not defeated(A,t), _parallel = 1 : _parallel != 2.
