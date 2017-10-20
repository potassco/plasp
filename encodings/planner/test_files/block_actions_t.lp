#program block_base.

#program block_step(t).
:- occurs(A,T1) : _test(_error(A,T2),M);
   _test(_error(_,T2),M);
   T1 = t.

#program block_model(m,length).
:- occurs(A,T1) : _test(_error(A,T2),M);
   _test(_error(_,T2),M);
   M = m;
   T1 = 1..length.

