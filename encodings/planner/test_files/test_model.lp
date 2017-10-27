% A1 disables A2
disable(A1,A2,T) :- occurs(A1,T), occurs(A2,T), A1 != A2,
                    postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.
% perform A1 at T
perform(A1,T) :- occurs(A1,T), perform(A2,T) : disable(A1,A2,T).

% test error
_error(A,T) :- occurs(A,T), not perform(A,T).
:- not _error(_,_).

% display plan
#show occurs/2.

