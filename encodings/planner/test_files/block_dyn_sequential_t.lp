#program block_base.

#program block_step(t).
:- min(Model,M),
   M { occurs(A,T) },
   T = t,
   not _external_dyn_sequential(Model).

#program block_model(m,length).

% error actions at T is N
sum(m,T,N) :- N = #sum { 1,A: _test(_error(A,T),m) },
              _test(_error(_,T),m).

% the minimum number of error actions per time point is M
min(m,M) :- M = #min { N: sum(m,T,N) }.

% force less than the minimum number of actions per time point
:- min(m,M),
   M { occurs(A,T) },
   T = 1..length,
   not _external_dyn_sequential(m).

% undo previous external and declare new one
_external_dyn_sequential(m-1).
#external _external_dyn_sequential(m).
