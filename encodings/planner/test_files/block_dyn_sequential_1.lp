#program block_base.

#program block_step(t).

#program block_model(m,length).

% error actions at T is N
sum(m,T,N) :- N = #sum { 1,A: _test(_error(A,T),m) },
              _test(_error(_,T),m).

% the minimum number of error actions per time point is M
min(m,M) :- M = #min { N: sum(m,T,N) }.

% add constraint for time T
activate(m,M,T) :- min(m,M), sum(m,T,_),
                   M<MM : activate(Model,MM,T), Model<m.

% force less than the minimum number of actions per time point
:- M { occurs(A,T) },
   activate(m,M,T).
