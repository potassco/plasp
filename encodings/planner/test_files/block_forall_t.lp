#program block_base.
selfdefeat(A,X) :- action(A), precondition(A,X,V),
                       postcondition(A,X,_),
                   not postcondition(A,X,V).

#program block_step(t).
single(X,T) :- occurs(A,T), selfdefeat(A,X),
               T=t.
:- single(X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1,
   T=t.

#program block_model(m,length).
single(X,T) :- occurs(A,T), selfdefeat(A,X),
               T=1..length.
:- single(X,T),
   #count{A : occurs(A,T), postcondition(A,X,V), not precondition(A,X,V)} > 1,
   T=1.length.
