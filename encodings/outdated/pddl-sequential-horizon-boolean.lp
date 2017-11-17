#const horizon=1.

step(0..horizon).

 precondition(Action,variable(Fluent),Value) :-  precondition(Action,                      variable(Fluent),value(Fluent,Value)).
postcondition(Action,variable(Fluent),Value) :- postcondition(Action,effect(unconditional),variable(Fluent),value(Fluent,Value)).

holds(variable(Fluent),0) :- initialState(variable(Fluent),value(Fluent,true)).

 { occurs(Action,T) : action(Action) } = 1 :- step(T), T>0.

:- occurs(Action,T), precondition(Action,Fluent,true), not holds(Fluent,T-1).
:- occurs(Action,T), precondition(Action,Fluent,false),    holds(Fluent,T-1).

caused(Fluent,T) :- occurs(Action,T), postcondition(Action,Fluent,true).

holds(Fluent,T) :- caused(Fluent,T).
holds(Fluent,T) :-  holds(Fluent,T-1), not occurs(Action,T) : postcondition(Action,Fluent,false); T<=horizon.

goalAt(T) :-     holds(variable(Variable),T) : goal(variable(Variable), value(Variable,true));
             not holds(variable(Variable),T) : goal(variable(Variable), value(Variable,false));
	     T=horizon.

 :- not goalAt(horizon).

#show occurs/2.
% #show holds/2.
