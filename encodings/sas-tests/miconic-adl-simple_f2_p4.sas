begin_version
3
end_version
begin_metric
0
end_metric
10
begin_variable
var0
-1
2
Atom boarded(p0)
NegatedAtom boarded(p0)
end_variable
begin_variable
var1
-1
2
Atom boarded(p1)
NegatedAtom boarded(p1)
end_variable
begin_variable
var2
-1
2
Atom boarded(p2)
NegatedAtom boarded(p2)
end_variable
begin_variable
var3
-1
2
Atom boarded(p3)
NegatedAtom boarded(p3)
end_variable
begin_variable
var4
-1
2
Atom lift-at(f0)
Atom lift-at(f1)
end_variable
begin_variable
var5
0
2
Atom new-axiom@0()
NegatedAtom new-axiom@0()
end_variable
begin_variable
var6
-1
2
Atom served(p0)
NegatedAtom served(p0)
end_variable
begin_variable
var7
-1
2
Atom served(p1)
NegatedAtom served(p1)
end_variable
begin_variable
var8
-1
2
Atom served(p2)
NegatedAtom served(p2)
end_variable
begin_variable
var9
-1
2
Atom served(p3)
NegatedAtom served(p3)
end_variable
1
begin_mutex_group
2
4 0
4 1
end_mutex_group
begin_state
1
1
1
1
0
0
1
1
1
1
end_state
begin_goal
1
5 1
end_goal
4
begin_operator
down f1 f0
0
1
0 4 1 0
1
end_operator
begin_operator
stop f0
1
4 0
7
0 0 -1 1
0 1 -1 1
0 2 -1 1
1 9 1 3 -1 0
1 0 0 6 -1 0
1 1 0 7 -1 0
1 2 0 8 -1 0
1
end_operator
begin_operator
stop f1
1
4 1
5
1 6 1 0 -1 0
1 7 1 1 -1 0
1 8 1 2 -1 0
0 3 -1 1
1 3 0 9 -1 0
1
end_operator
begin_operator
up f0 f1
0
1
0 4 0 1
1
end_operator
1
begin_rule
4
6 0
7 0
8 0
9 0
5 0 1
end_rule
