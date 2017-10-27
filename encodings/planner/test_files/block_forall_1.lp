#program block_base.

selfdefeat(A,X) :- action(A), precondition(A,X,V),
                       postcondition(A,X,_),
                   not postcondition(A,X,V).

#program block_step(t).

#program block_model(m,length).

_error_at(m,T)   :- _test(_error(A,T),m).

single(m,X,T) :- occurs(A,T), selfdefeat(A,X),
                 _error_at(m,T).

:- single(m,X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1.
