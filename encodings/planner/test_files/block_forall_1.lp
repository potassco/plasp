#program block_base.

selfdefeat(A,X) :- active(A), precondition(A,X,V),
                   has_condition(A,X,1), not postcondition(A,X,V).

#program block_step(t).

#program block_model(m,length).

_error_at(m,T)   :- _test(_error(A,T),m).

:- occurs(A,T), precondition(A,X,V), not has_condition(A,X,1), not holds(X,V,T).
   _error_at(m,T).

single(m,X,T) :- occurs(A,T), selfdefeat(A,X),
                 _error_at(m,T).

:- single(m,X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1.
