#program block_base.
% ADAPT
active(A) :- action(A).
disable(A1,A2) :- action(A1), action(A2), A1 != A2,
                  postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.

#program block_step(t).

#program block_model(m,t).
time(T,m) :- _test(_error(A,T),m).

perform(A,T) :- active(A), time(T,m),
                alright(A1,T) : disable(A,A1).

alright(A,T) :- active(A), not occurs(A,T), time(T,m).
alright(A,T) :- perform(A,T), time(T,m).

:- active(A), not alright(A,T), time(T,m).

