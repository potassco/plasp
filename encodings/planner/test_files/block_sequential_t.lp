#program block_base.

#program block_step(t).
:- 2 { occurs(A,T) }, T=t.

#program block_model(m,length).
:- 2 { occurs(A,T) }, T=1..length.

