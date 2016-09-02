begin_version
3
end_version
begin_metric
1
end_metric
21
begin_variable
var0
-1
2
Atom made(p1)
NegatedAtom made(p1)
end_variable
begin_variable
var1
-1
2
Atom made(p2)
NegatedAtom made(p2)
end_variable
begin_variable
var2
-1
2
Atom made(p3)
NegatedAtom made(p3)
end_variable
begin_variable
var3
-1
2
Atom made(p4)
NegatedAtom made(p4)
end_variable
begin_variable
var4
-1
2
Atom made(p5)
NegatedAtom made(p5)
end_variable
begin_variable
var5
0
2
Atom new-axiom@0(p1)
NegatedAtom new-axiom@0(p1)
end_variable
begin_variable
var6
0
2
Atom new-axiom@0(p2)
NegatedAtom new-axiom@0(p2)
end_variable
begin_variable
var7
0
2
Atom new-axiom@0(p3)
NegatedAtom new-axiom@0(p3)
end_variable
begin_variable
var8
0
2
Atom new-axiom@0(p4)
NegatedAtom new-axiom@0(p4)
end_variable
begin_variable
var9
0
2
Atom new-axiom@0(p5)
NegatedAtom new-axiom@0(p5)
end_variable
begin_variable
var10
0
2
Atom new-axiom@1(o1)
NegatedAtom new-axiom@1(o1)
end_variable
begin_variable
var11
0
2
Atom new-axiom@1(o2)
NegatedAtom new-axiom@1(o2)
end_variable
begin_variable
var12
0
2
Atom new-axiom@1(o3)
NegatedAtom new-axiom@1(o3)
end_variable
begin_variable
var13
0
2
Atom new-axiom@1(o4)
NegatedAtom new-axiom@1(o4)
end_variable
begin_variable
var14
0
2
Atom new-axiom@1(o5)
NegatedAtom new-axiom@1(o5)
end_variable
begin_variable
var15
-1
3
Atom shipped(o1)
Atom started(o1)
Atom waiting(o1)
end_variable
begin_variable
var16
-1
3
Atom shipped(o2)
Atom started(o2)
Atom waiting(o2)
end_variable
begin_variable
var17
-1
3
Atom shipped(o3)
Atom started(o3)
Atom waiting(o3)
end_variable
begin_variable
var18
-1
3
Atom shipped(o4)
Atom started(o4)
Atom waiting(o4)
end_variable
begin_variable
var19
-1
3
Atom shipped(o5)
Atom started(o5)
Atom waiting(o5)
end_variable
begin_variable
var20
-1
6
Atom stacks-avail(n0)
Atom stacks-avail(n1)
Atom stacks-avail(n2)
Atom stacks-avail(n3)
Atom stacks-avail(n4)
Atom stacks-avail(n5)
end_variable
11
begin_mutex_group
3
15 0
15 1
15 2
end_mutex_group
begin_mutex_group
3
16 0
16 1
16 2
end_mutex_group
begin_mutex_group
3
17 0
17 1
17 2
end_mutex_group
begin_mutex_group
3
18 0
18 1
18 2
end_mutex_group
begin_mutex_group
3
19 0
19 1
19 2
end_mutex_group
begin_mutex_group
6
20 0
20 1
20 2
20 3
20 4
20 5
end_mutex_group
begin_mutex_group
2
15 1
15 2
end_mutex_group
begin_mutex_group
2
16 1
16 2
end_mutex_group
begin_mutex_group
2
17 1
17 2
end_mutex_group
begin_mutex_group
2
18 1
18 2
end_mutex_group
begin_mutex_group
2
19 1
19 2
end_mutex_group
begin_state
1
1
1
1
1
0
0
0
0
0
0
0
0
0
0
2
2
2
2
2
0
end_state
begin_goal
5
15 0
16 0
17 0
18 0
19 0
end_goal
60
begin_operator
make-product p1
1
5 1
1
0 0 1 0
0
end_operator
begin_operator
make-product p2
1
6 1
1
0 1 1 0
0
end_operator
begin_operator
make-product p3
1
7 1
1
0 2 1 0
0
end_operator
begin_operator
make-product p4
1
8 1
1
0 3 1 0
0
end_operator
begin_operator
make-product p5
1
9 1
1
0 4 1 0
0
end_operator
begin_operator
open-new-stack n0 n1
0
1
0 20 0 1
1
end_operator
begin_operator
open-new-stack n1 n2
0
1
0 20 1 2
1
end_operator
begin_operator
open-new-stack n2 n3
0
1
0 20 2 3
1
end_operator
begin_operator
open-new-stack n3 n4
0
1
0 20 3 4
1
end_operator
begin_operator
open-new-stack n4 n5
0
1
0 20 4 5
1
end_operator
begin_operator
ship-order o1 n0 n1
1
10 1
2
0 15 1 0
0 20 0 1
0
end_operator
begin_operator
ship-order o1 n1 n2
1
10 1
2
0 15 1 0
0 20 1 2
0
end_operator
begin_operator
ship-order o1 n2 n3
1
10 1
2
0 15 1 0
0 20 2 3
0
end_operator
begin_operator
ship-order o1 n3 n4
1
10 1
2
0 15 1 0
0 20 3 4
0
end_operator
begin_operator
ship-order o1 n4 n5
1
10 1
2
0 15 1 0
0 20 4 5
0
end_operator
begin_operator
ship-order o2 n0 n1
1
11 1
2
0 16 1 0
0 20 0 1
0
end_operator
begin_operator
ship-order o2 n1 n2
1
11 1
2
0 16 1 0
0 20 1 2
0
end_operator
begin_operator
ship-order o2 n2 n3
1
11 1
2
0 16 1 0
0 20 2 3
0
end_operator
begin_operator
ship-order o2 n3 n4
1
11 1
2
0 16 1 0
0 20 3 4
0
end_operator
begin_operator
ship-order o2 n4 n5
1
11 1
2
0 16 1 0
0 20 4 5
0
end_operator
begin_operator
ship-order o3 n0 n1
1
12 1
2
0 17 1 0
0 20 0 1
0
end_operator
begin_operator
ship-order o3 n1 n2
1
12 1
2
0 17 1 0
0 20 1 2
0
end_operator
begin_operator
ship-order o3 n2 n3
1
12 1
2
0 17 1 0
0 20 2 3
0
end_operator
begin_operator
ship-order o3 n3 n4
1
12 1
2
0 17 1 0
0 20 3 4
0
end_operator
begin_operator
ship-order o3 n4 n5
1
12 1
2
0 17 1 0
0 20 4 5
0
end_operator
begin_operator
ship-order o4 n0 n1
1
13 1
2
0 18 1 0
0 20 0 1
0
end_operator
begin_operator
ship-order o4 n1 n2
1
13 1
2
0 18 1 0
0 20 1 2
0
end_operator
begin_operator
ship-order o4 n2 n3
1
13 1
2
0 18 1 0
0 20 2 3
0
end_operator
begin_operator
ship-order o4 n3 n4
1
13 1
2
0 18 1 0
0 20 3 4
0
end_operator
begin_operator
ship-order o4 n4 n5
1
13 1
2
0 18 1 0
0 20 4 5
0
end_operator
begin_operator
ship-order o5 n0 n1
1
14 1
2
0 19 1 0
0 20 0 1
0
end_operator
begin_operator
ship-order o5 n1 n2
1
14 1
2
0 19 1 0
0 20 1 2
0
end_operator
begin_operator
ship-order o5 n2 n3
1
14 1
2
0 19 1 0
0 20 2 3
0
end_operator
begin_operator
ship-order o5 n3 n4
1
14 1
2
0 19 1 0
0 20 3 4
0
end_operator
begin_operator
ship-order o5 n4 n5
1
14 1
2
0 19 1 0
0 20 4 5
0
end_operator
begin_operator
start-order o1 n1 n0
0
2
0 15 2 1
0 20 1 0
0
end_operator
begin_operator
start-order o1 n2 n1
0
2
0 15 2 1
0 20 2 1
0
end_operator
begin_operator
start-order o1 n3 n2
0
2
0 15 2 1
0 20 3 2
0
end_operator
begin_operator
start-order o1 n4 n3
0
2
0 15 2 1
0 20 4 3
0
end_operator
begin_operator
start-order o1 n5 n4
0
2
0 15 2 1
0 20 5 4
0
end_operator
begin_operator
start-order o2 n1 n0
0
2
0 16 2 1
0 20 1 0
0
end_operator
begin_operator
start-order o2 n2 n1
0
2
0 16 2 1
0 20 2 1
0
end_operator
begin_operator
start-order o2 n3 n2
0
2
0 16 2 1
0 20 3 2
0
end_operator
begin_operator
start-order o2 n4 n3
0
2
0 16 2 1
0 20 4 3
0
end_operator
begin_operator
start-order o2 n5 n4
0
2
0 16 2 1
0 20 5 4
0
end_operator
begin_operator
start-order o3 n1 n0
0
2
0 17 2 1
0 20 1 0
0
end_operator
begin_operator
start-order o3 n2 n1
0
2
0 17 2 1
0 20 2 1
0
end_operator
begin_operator
start-order o3 n3 n2
0
2
0 17 2 1
0 20 3 2
0
end_operator
begin_operator
start-order o3 n4 n3
0
2
0 17 2 1
0 20 4 3
0
end_operator
begin_operator
start-order o3 n5 n4
0
2
0 17 2 1
0 20 5 4
0
end_operator
begin_operator
start-order o4 n1 n0
0
2
0 18 2 1
0 20 1 0
0
end_operator
begin_operator
start-order o4 n2 n1
0
2
0 18 2 1
0 20 2 1
0
end_operator
begin_operator
start-order o4 n3 n2
0
2
0 18 2 1
0 20 3 2
0
end_operator
begin_operator
start-order o4 n4 n3
0
2
0 18 2 1
0 20 4 3
0
end_operator
begin_operator
start-order o4 n5 n4
0
2
0 18 2 1
0 20 5 4
0
end_operator
begin_operator
start-order o5 n1 n0
0
2
0 19 2 1
0 20 1 0
0
end_operator
begin_operator
start-order o5 n2 n1
0
2
0 19 2 1
0 20 2 1
0
end_operator
begin_operator
start-order o5 n3 n2
0
2
0 19 2 1
0 20 3 2
0
end_operator
begin_operator
start-order o5 n4 n3
0
2
0 19 2 1
0 20 4 3
0
end_operator
begin_operator
start-order o5 n5 n4
0
2
0 19 2 1
0 20 5 4
0
end_operator
10
begin_rule
2
0 0
1 0
11 0 1
end_rule
begin_rule
1
1 0
10 0 1
end_rule
begin_rule
1
2 0
12 0 1
end_rule
begin_rule
2
2 0
3 0
13 0 1
end_rule
begin_rule
1
4 0
14 0 1
end_rule
begin_rule
2
15 1
16 1
6 0 1
end_rule
begin_rule
1
16 1
5 0 1
end_rule
begin_rule
2
17 1
18 1
7 0 1
end_rule
begin_rule
1
18 1
8 0 1
end_rule
begin_rule
1
19 1
9 0 1
end_rule
