% Additional rules for enforcing the inclusion of parallel actions in plans,
% whenever such 'redundant' actions are compatible with states and other actions

compatible(A,A1)  :- active(A), active(A1), A != A1, _parallel = 2,
                     not diverge(A,A1), not diverge(A1,A).
disable(A,A1,A2)  :- disable(A1,A2), compatible(A,A1), compatible(A,A2).
disabled(A,A2)    :- disable(A,A1,A2).
disabled(A,A2)    :- disable(A,A2), _parallel = 2.

#program step(t).

defeated(A,t) :- active(A), postcondition(A,X,V), fluent(X), not holds(X,V,t),
                 _parallel = 1 : _parallel != 2.

defeated(A,t) :- _parallel = 1, active(A), precondition(A,X,V), not holds(X,V,t-1).
defeated(A,t) :- _parallel = 1, active(A), precondition(A,X,V), not holds(X,V,t).
defeated(A,t) :- _parallel = 1, active(A), postcondition(A,X,V), not precondition(A,X,V),
                 single(X,t).

proceed(A,X,V,t) :- active(A), holds(X,V,t-1), scope(X,V).
proceed(A,X,V,t) :- compatible(A,A1), occurs(A1,t), perform(A,A1,t),
                    postcondition(A1,X,V), scope(X,V), not precondition(A1,X,V).

perform(A,A1,t) :- disabled(A,A1), not occurs(A1,t).
perform(A,A1,t) :- compatible(A,A1),
                   proceed(A,X,V,t) : precondition(A1,X,V); perform(A,A2,t) : disable(A,A1,A2).
perform(A,A,t)  :- active(A), _parallel = 2,
                   proceed(A,X,V,t) : precondition(A,X,V); perform(A,A2,t) : disable(A,A2).

defeated(A,t) :- _parallel = 2, active(A), not perform(A,A,t).

:- active(A), not occurs(A,t), not defeated(A,t), _parallel = 1 : _parallel != 2.
