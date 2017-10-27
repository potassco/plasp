#program block_base.
disable(A1,A2) :- active(A1), active(A2), A1 != A2, postcondition(A1,X,V),
                  has_condition(A2,X,0), not precondition(A2,X,V),
                  not diverge(A1,A2), not exclude(A1,A2).

#program block_step(t).

#program block_model(m,t).
time(T,m) :- _test(_error(A,T),m).

perform(A,T) :- active(A), time(T,m),
                alright(A1,T) : disable(A,A1).

alright(A,T) :- active(A), not occurs(A,T), time(T,m).
alright(A,T) :- perform(A,T), time(T,m).

:- active(A), not alright(A,T), time(T,m).

