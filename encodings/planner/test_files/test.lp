% Constant test to select option:
% - value '1': return all non serializable actions (default)
% - value '2': return one cardinality minimal set of non-serializable actions
%
test(X) :- test = X, X=1..2.
test(1) :- test != 1, test != 2.

% A1 disables A2 at T
disable(A1,A2,T) :- occurs(A1,T), occurs(A2,T), A1 != A2,
                    postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.
% perform A1 at T
perform(A1,T) :- occurs(A1,T), perform(A2,T) : disable(A1,A2,T).

%
% test 1
%
_error(A,T) :- occurs(A,T), not perform(A,T), test(1).
:- not _error(_,_),                           test(1).

%
% test 2
%

% select not perfomed actions from exactly one time point
{ _error(A,T) : occurs(A,T), not perform(A,T) } :- test(2).
:- not 1 #sum { 1,T : _error(A,T) } 1,             test(2).

% transitive closure of the selected actions
edge(A1,A2,T) :- _error(A1,T), _error(A2,T), disable(A1,A2,T), test(2).
tr(A1,A2,T)   :- edge(A1,A2,T),                                test(2).
tr(A1,A3,T)   :- edge(A1,A2,T), tr(A2,A3,T),                   test(2).

% there must be a loop
loop :- tr(A,A,T), test(2).
:- not loop,       test(2).

% minimize
#minimize{ 1,A,T: _error(A,T), test(2) }.

% DISPLAY
#show _error/2.
