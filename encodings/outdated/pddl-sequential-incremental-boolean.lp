#include <incmode>.

#program base.

 precondition(Action,variable(Fluent),Value) :-  precondition(Action,                      variable(Fluent),value(Fluent,Value)).
postcondition(Action,variable(Fluent),Value) :- postcondition(Action,effect(unconditional),variable(Fluent),value(Fluent,Value)).

holds(variable(Fluent),0) :- initialState(variable(Fluent),value(Fluent,true)).

#program step(t).

 { occurs(Action,t) : action(Action) } = 1.

:- occurs(Action,t), precondition(Action,Fluent,true), not holds(Fluent,t-1).
:- occurs(Action,t), precondition(Action,Fluent,false),    holds(Fluent,t-1).

caused(Fluent,t) :- occurs(Action,t), postcondition(Action,Fluent,true).

holds(Fluent,t) :- caused(Fluent,t).
holds(Fluent,t) :-  holds(Fluent,t-1), not occurs(Action,t) : postcondition(Action,Fluent,false).

#program check(t).

goalAt(t) :-     holds(variable(Variable),t) : goal(variable(Variable), value(Variable,true));
             not holds(variable(Variable),t) : goal(variable(Variable), value(Variable,false)).

 :- not goalAt(t), query(t).

#show occurs/2.
% #show holds/2.
