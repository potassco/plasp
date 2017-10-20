#program block_base.
% ADAPT
active(A) :- action(A).
disable(A1,A2) :- action(A1), action(A2), A1 != A2,
                  postcondition(A1,_,X,V1), precondition(A2,X,V2), V1 != V2.

#program block_step(t).
perform(A,T) :- active(A), T=t,
                alright(A1,T) : disable(A,A1).

alright(A,T) :- active(A), not occurs(A,T), T=t.
alright(A,T) :- perform(A,T), T=t.

:- active(A), not alright(A,T), T=t.

#program block_model(m,length).
perform(A,T) :- active(A), T=1..length,
                alright(A1,T) : disable(A,A1).

alright(A,T) :- active(A), not occurs(A,T), T=1..length.
alright(A,T) :- perform(A,T), T=1..length.

:- active(A), not alright(A,T), T=1..length.

