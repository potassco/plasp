#program block_base.

#program block_step(t).

#program block_model(m,length).
:- occurs(A,T) : _test(occurs(A,T),m);
   _test(occurs(A,T),m) : occurs(A,T).
