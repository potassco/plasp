#program block_base.

#program block_step(t).

#program block_model(m,length).
:- occurs(A,T1) : _test(_error(A,T2),m);
   _test(_error(_,T2),m);
   T1 = T2.

