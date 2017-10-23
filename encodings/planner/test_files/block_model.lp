#program block_base.

#program block_step(t).

#program block_model(m,length).
:- occurs(A,T) : _test(_error(A,T),m);
   _test(_error(A,T),m) : occurs(A,T).
