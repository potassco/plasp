#program block_base.

% as in strips-incremental.lp
#const _block_shallow=0.

diverge(A1,A2,X) :- active(A1), active(A2), postcondition(A1,X,V),
                    has_condition(A2,X,1), not postcondition(A2,X,V),
                    _block_shallow = 0.

diverge(A1,A2)   :- diverge(A1,A2,X).

exclude(A1,A2) :- active(A1), active(A2), precondition(A1,X,V),
                  has_condition(A2,X,0), not precondition(A2,X,V),
                  _block_shallow = 0.

disable(A1,A2) :- active(A1), active(A2), A1 != A2, postcondition(A1,X,V),
                  has_condition(A2,X,0), not precondition(A2,X,V),
                  not diverge(A1,A2), not exclude(A1,A2).

disable :- #sum{1,A1 : disable(A1,A2); -1,A2 : disable(A1,A2)} > 0.

#program block_step(t).
#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A1,T), not disable, T=t.
#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A2,T),     disable, T=t.

#program block_model(m,length).
#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A1,T), not disable, T=1..length.
#edge((A1,T),(A2,T)) : disable(A1,A2), occurs(A2,T),     disable, T=1..length.

