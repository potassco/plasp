% Convert a plan (possibly with parallel actions), given by atoms over occurs/2,
% to a sequential plan, expressed by atoms over sequence/2

time(T) :- occurs(A,T).
time    :- time(T).

:- goal(X,V), not initialState(X,V), not time.

last(T2) :- time, T2 = #max{T1 : time(T1)}.

offset(T2,M)    :- time(T2), M = #count{A,T1 : occurs(A,T1), T1 < T2}.
finish(T,M+N)   :- offset(T,M), N = #count{A : occurs(A,T)}.
index(T,M+1..N) :- offset(T,M), finish(T,N).

postcondition(A,X,V) :- postcondition(A,E,X,V), occurs(A,T).
postcondition(A,X)   :- postcondition(A,X,V).

before(A1,A2,T) :- occurs(A1,T), occurs(A2,T), A1 != A2,
                   precondition(A1,X,V), postcondition(A2,X), not postcondition(A2,X,V).

order(A1,A2,T) :- occurs(A1,T), occurs(A2,T), A1 < A2, A <= A1 : occurs(A,T), A < A2.
first(A2,T)    :- occurs(A2,T), #false : order(A1,A2,T).

undone(A,T,M) :- occurs(A,T), offset(T,M).
undone(A,T,N) :- undone(A,T,N-1), select(A1,T,N), A != A1, index(T,N+1).

done(A,T,N) :- select(A,T,N).
done(A,T,N) :- done(A,T,N-1), index(T,N).

:- finish(T,N), occurs(A,T), not done(A,T,N).

hold(X,V,0) :- initialState(X,V).
hold(X,V,N) :- select(A,T,N), postcondition(A,X,V).
hold(X,V,N) :- select(A,T,N), hold(X,V,N-1), not postcondition(A,X).

:- last(T), finish(T,N), goal(X,V), not hold(X,V,N).

hold(X,N) :- hold(X,V,N).

:- hold(X,N), #count{V : hold(X,V,N)} > 1.

preconditions(A,T,N) :- undone(A,T,N), hold(X,V,N) : precondition(A,X,V).
applicable(A,T,N)    :- preconditions(A,T,N), done(A1,T,N) : before(A1,A,T).

inapplicable(A,T,N) :- done(A,T,N), index(T,N+1).
inapplicable(A,T,N) :- undone(A,T,N), precondition(A,X,V1), hold(X,V2,N), V1 != V2.
inapplicable(A,T,N) :- undone(A1,T,N), before(A1,A,T).

continue(A2,T,N) :- order(A1,A2,T), inapplicable(A1,T,N), first(A1,T).
continue(A2,T,N) :- order(A1,A2,T), inapplicable(A1,T,N), continue(A1,T,N).

select(A,T,N+1) :- applicable(A,T,N), first(A,T).
select(A,T,N+1) :- applicable(A,T,N), continue(A,T,N).

% DISPLAY PART

#show.
#show sequence(A,N) : select(A,T,N).
