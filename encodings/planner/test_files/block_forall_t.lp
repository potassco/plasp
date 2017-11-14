#program block_base.

selfdefeat(A,X) :- active(A), precondition(A,X,V),
                   has_condition(A,X,1), not postcondition(A,X,V).

#program block_step(t).

:- occurs(A,T), precondition(A,X,V), not has_condition(A,X,1), not holds(X,V,T),
   T=t.

single(X,T) :- occurs(A,T), selfdefeat(A,X),
               T=t.
:- single(X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1,
   T=t.

#program block_model(m,length).

:- occurs(A,T), precondition(A,X,V), not has_condition(A,X,1), not holds(X,V,T),
   T=1..length.

single(X,T) :- occurs(A,T), selfdefeat(A,X),
               T=1..length.
:- single(X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1,
   T=1..length.

