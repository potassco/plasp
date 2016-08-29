begin_version
3
end_version
begin_metric
0
end_metric
29
begin_variable
var0
-1
14
Atom activate(philosopher-0, forks--pid-rfork)
Atom activate(philosopher-0, forks--pid-wfork)
Atom activate(philosopher-0, forks-__-pidp1__2_-rfork)
Atom activate(philosopher-0, forks-__-pidp1__2_-wfork)
Atom blocked-trans(philosopher-0, forks--pid-rfork)
Atom blocked-trans(philosopher-0, forks--pid-wfork)
Atom blocked-trans(philosopher-0, forks-__-pidp1__2_-rfork)
Atom blocked-trans(philosopher-0, forks-__-pidp1__2_-wfork)
Atom enabled(philosopher-0, forks--pid-rfork)
Atom enabled(philosopher-0, forks--pid-wfork)
Atom enabled(philosopher-0, forks-__-pidp1__2_-rfork)
Atom enabled(philosopher-0, forks-__-pidp1__2_-wfork)
Atom pending(philosopher-0)
<none of those>
end_variable
begin_variable
var1
-1
14
Atom activate(philosopher-1, forks--pid-rfork)
Atom activate(philosopher-1, forks--pid-wfork)
Atom activate(philosopher-1, forks-__-pidp1__2_-rfork)
Atom activate(philosopher-1, forks-__-pidp1__2_-wfork)
Atom blocked-trans(philosopher-1, forks--pid-rfork)
Atom blocked-trans(philosopher-1, forks--pid-wfork)
Atom blocked-trans(philosopher-1, forks-__-pidp1__2_-rfork)
Atom blocked-trans(philosopher-1, forks-__-pidp1__2_-wfork)
Atom enabled(philosopher-1, forks--pid-rfork)
Atom enabled(philosopher-1, forks--pid-wfork)
Atom enabled(philosopher-1, forks-__-pidp1__2_-rfork)
Atom enabled(philosopher-1, forks-__-pidp1__2_-wfork)
Atom pending(philosopher-1)
<none of those>
end_variable
begin_variable
var2
-1
4
Atom advance-head(forks-0-)
Atom advance-tail(forks-0-)
Atom settled(forks-0-)
<none of those>
end_variable
begin_variable
var3
-1
4
Atom advance-head(forks-1-)
Atom advance-tail(forks-1-)
Atom settled(forks-1-)
<none of those>
end_variable
begin_variable
var4
-1
5
Atom at-process(philosopher-0, state-1)
Atom at-process(philosopher-0, state-3)
Atom at-process(philosopher-0, state-4)
Atom at-process(philosopher-0, state-5)
Atom at-process(philosopher-0, state-6)
end_variable
begin_variable
var5
-1
5
Atom at-process(philosopher-1, state-1)
Atom at-process(philosopher-1, state-3)
Atom at-process(philosopher-1, state-4)
Atom at-process(philosopher-1, state-5)
Atom at-process(philosopher-1, state-6)
end_variable
begin_variable
var6
-1
2
Atom blocked(philosopher-0)
NegatedAtom blocked(philosopher-0)
end_variable
begin_variable
var7
-1
2
Atom blocked(philosopher-1)
NegatedAtom blocked(philosopher-1)
end_variable
begin_variable
var8
0
2
Atom new-axiom@0(philosopher-0, philosopher, state-1)
NegatedAtom new-axiom@0(philosopher-0, philosopher, state-1)
end_variable
begin_variable
var9
0
2
Atom new-axiom@0(philosopher-0, philosopher, state-3)
NegatedAtom new-axiom@0(philosopher-0, philosopher, state-3)
end_variable
begin_variable
var10
0
2
Atom new-axiom@0(philosopher-0, philosopher, state-4)
NegatedAtom new-axiom@0(philosopher-0, philosopher, state-4)
end_variable
begin_variable
var11
0
2
Atom new-axiom@0(philosopher-0, philosopher, state-5)
NegatedAtom new-axiom@0(philosopher-0, philosopher, state-5)
end_variable
begin_variable
var12
0
2
Atom new-axiom@0(philosopher-0, philosopher, state-6)
NegatedAtom new-axiom@0(philosopher-0, philosopher, state-6)
end_variable
begin_variable
var13
0
2
Atom new-axiom@0(philosopher-1, philosopher, state-1)
NegatedAtom new-axiom@0(philosopher-1, philosopher, state-1)
end_variable
begin_variable
var14
0
2
Atom new-axiom@0(philosopher-1, philosopher, state-3)
NegatedAtom new-axiom@0(philosopher-1, philosopher, state-3)
end_variable
begin_variable
var15
0
2
Atom new-axiom@0(philosopher-1, philosopher, state-4)
NegatedAtom new-axiom@0(philosopher-1, philosopher, state-4)
end_variable
begin_variable
var16
0
2
Atom new-axiom@0(philosopher-1, philosopher, state-5)
NegatedAtom new-axiom@0(philosopher-1, philosopher, state-5)
end_variable
begin_variable
var17
0
2
Atom new-axiom@0(philosopher-1, philosopher, state-6)
NegatedAtom new-axiom@0(philosopher-1, philosopher, state-6)
end_variable
begin_variable
var18
0
2
Atom new-axiom@1()
NegatedAtom new-axiom@1()
end_variable
begin_variable
var19
-1
2
Atom queue-head-msg(forks-0-, empty)
NegatedAtom queue-head-msg(forks-0-, empty)
end_variable
begin_variable
var20
-1
2
Atom queue-head-msg(forks-0-, fork)
NegatedAtom queue-head-msg(forks-0-, fork)
end_variable
begin_variable
var21
-1
2
Atom queue-head-msg(forks-1-, empty)
NegatedAtom queue-head-msg(forks-1-, empty)
end_variable
begin_variable
var22
-1
2
Atom queue-head-msg(forks-1-, fork)
NegatedAtom queue-head-msg(forks-1-, fork)
end_variable
begin_variable
var23
-1
2
Atom queue-msg(forks-0-, qs-0, fork)
NegatedAtom queue-msg(forks-0-, qs-0, fork)
end_variable
begin_variable
var24
-1
2
Atom queue-msg(forks-1-, qs-0, fork)
NegatedAtom queue-msg(forks-1-, qs-0, fork)
end_variable
begin_variable
var25
-1
2
Atom queue-size(forks-0-, one)
Atom queue-size(forks-0-, zero)
end_variable
begin_variable
var26
-1
2
Atom queue-size(forks-1-, one)
Atom queue-size(forks-1-, zero)
end_variable
begin_variable
var27
-1
2
Atom queue-tail-msg(forks-0-, fork)
NegatedAtom queue-tail-msg(forks-0-, fork)
end_variable
begin_variable
var28
-1
2
Atom queue-tail-msg(forks-1-, fork)
NegatedAtom queue-tail-msg(forks-1-, fork)
end_variable
10
begin_mutex_group
13
0 0
0 1
0 2
0 3
0 4
0 5
0 6
0 7
0 8
0 9
0 10
0 11
0 12
end_mutex_group
begin_mutex_group
9
0 0
0 1
0 2
0 3
0 8
0 9
0 10
0 11
0 12
end_mutex_group
begin_mutex_group
13
1 0
1 1
1 2
1 3
1 4
1 5
1 6
1 7
1 8
1 9
1 10
1 11
1 12
end_mutex_group
begin_mutex_group
9
1 0
1 1
1 2
1 3
1 8
1 9
1 10
1 11
1 12
end_mutex_group
begin_mutex_group
3
2 0
2 1
2 2
end_mutex_group
begin_mutex_group
3
3 0
3 1
3 2
end_mutex_group
begin_mutex_group
5
4 0
4 1
4 2
4 3
4 4
end_mutex_group
begin_mutex_group
5
5 0
5 1
5 2
5 3
5 4
end_mutex_group
begin_mutex_group
2
25 0
25 1
end_mutex_group
begin_mutex_group
2
26 0
26 1
end_mutex_group
begin_state
12
12
2
2
0
0
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
0
0
1
0
1
1
1
1
1
1
1
end_state
begin_goal
2
6 0
7 0
end_goal
56
begin_operator
activate-trans philosopher-0 philosopher forks--pid-rfork state-6 state-3
2
4 4
18 1
1
0 0 12 0
1
end_operator
begin_operator
activate-trans philosopher-0 philosopher forks--pid-wfork state-1 state-6
2
4 0
18 1
1
0 0 12 1
1
end_operator
begin_operator
activate-trans philosopher-0 philosopher forks--pid-wfork state-4 state-5
2
4 2
18 1
1
0 0 12 1
1
end_operator
begin_operator
activate-trans philosopher-0 philosopher forks-__-pidp1__2_-rfork state-3 state-4
2
4 1
18 1
1
0 0 12 2
1
end_operator
begin_operator
activate-trans philosopher-0 philosopher forks-__-pidp1__2_-wfork state-5 state-6
2
4 3
18 1
1
0 0 12 3
1
end_operator
begin_operator
activate-trans philosopher-1 philosopher forks--pid-rfork state-6 state-3
2
5 4
18 1
1
0 1 12 0
1
end_operator
begin_operator
activate-trans philosopher-1 philosopher forks--pid-wfork state-1 state-6
2
5 0
18 1
1
0 1 12 1
1
end_operator
begin_operator
activate-trans philosopher-1 philosopher forks--pid-wfork state-4 state-5
2
5 2
18 1
1
0 1 12 1
1
end_operator
begin_operator
activate-trans philosopher-1 philosopher forks-__-pidp1__2_-rfork state-3 state-4
2
5 1
18 1
1
0 1 12 2
1
end_operator
begin_operator
activate-trans philosopher-1 philosopher forks-__-pidp1__2_-wfork state-5 state-6
2
5 3
18 1
1
0 1 12 3
1
end_operator
begin_operator
advance-empty-queue-tail forks-0- queue-1 qs-0 qs-0 fork empty zero one
1
27 0
5
0 2 1 2
0 19 0 1
0 20 -1 0
0 23 -1 0
0 25 1 0
1
end_operator
begin_operator
advance-empty-queue-tail forks-0- queue-1 qs-0 qs-0 fork fork zero one
2
20 0
27 0
3
0 2 1 2
0 23 -1 0
0 25 1 0
1
end_operator
begin_operator
advance-empty-queue-tail forks-1- queue-1 qs-0 qs-0 fork empty zero one
1
28 0
5
0 3 1 2
0 21 0 1
0 22 -1 0
0 24 -1 0
0 26 1 0
1
end_operator
begin_operator
advance-empty-queue-tail forks-1- queue-1 qs-0 qs-0 fork fork zero one
2
22 0
28 0
3
0 3 1 2
0 24 -1 0
0 26 1 0
1
end_operator
begin_operator
advance-queue-head forks-0- queue-1 qs-0 qs-0 fork one zero
1
23 0
3
0 2 0 2
0 20 -1 0
0 25 0 1
1
end_operator
begin_operator
advance-queue-head forks-1- queue-1 qs-0 qs-0 fork one zero
1
24 0
3
0 3 0 2
0 22 -1 0
0 26 0 1
1
end_operator
begin_operator
block philosopher-0 state-1 philosopher
2
4 0
8 1
1
0 6 -1 0
1
end_operator
begin_operator
block philosopher-0 state-3 philosopher
2
4 1
9 1
1
0 6 -1 0
1
end_operator
begin_operator
block philosopher-0 state-4 philosopher
2
4 2
10 1
1
0 6 -1 0
1
end_operator
begin_operator
block philosopher-0 state-5 philosopher
2
4 3
11 1
1
0 6 -1 0
1
end_operator
begin_operator
block philosopher-0 state-6 philosopher
2
4 4
12 1
1
0 6 -1 0
1
end_operator
begin_operator
block philosopher-1 state-1 philosopher
2
5 0
13 1
1
0 7 -1 0
1
end_operator
begin_operator
block philosopher-1 state-3 philosopher
2
5 1
14 1
1
0 7 -1 0
1
end_operator
begin_operator
block philosopher-1 state-4 philosopher
2
5 2
15 1
1
0 7 -1 0
1
end_operator
begin_operator
block philosopher-1 state-5 philosopher
2
5 3
16 1
1
0 7 -1 0
1
end_operator
begin_operator
block philosopher-1 state-6 philosopher
2
5 4
17 1
1
0 7 -1 0
1
end_operator
begin_operator
block-read-queue-empty philosopher-0 forks--pid-rfork forks-0- fork zero
1
25 1
2
0 0 0 4
0 2 2 3
1
end_operator
begin_operator
block-read-queue-empty philosopher-0 forks-__-pidp1__2_-rfork forks-1- fork zero
1
26 1
2
0 0 2 6
0 3 2 3
1
end_operator
begin_operator
block-read-queue-empty philosopher-1 forks--pid-rfork forks-1- fork zero
1
26 1
2
0 1 0 4
0 3 2 3
1
end_operator
begin_operator
block-read-queue-empty philosopher-1 forks-__-pidp1__2_-rfork forks-0- fork zero
1
25 1
2
0 1 2 6
0 2 2 3
1
end_operator
begin_operator
block-read-wrong-message philosopher-0 forks--pid-rfork forks-0- fork empty
1
19 0
2
0 0 0 4
0 2 2 3
1
end_operator
begin_operator
block-read-wrong-message philosopher-0 forks-__-pidp1__2_-rfork forks-1- fork empty
1
21 0
2
0 0 2 6
0 3 2 3
1
end_operator
begin_operator
block-read-wrong-message philosopher-1 forks--pid-rfork forks-1- fork empty
1
21 0
2
0 1 0 4
0 3 2 3
1
end_operator
begin_operator
block-read-wrong-message philosopher-1 forks-__-pidp1__2_-rfork forks-0- fork empty
1
19 0
2
0 1 2 6
0 2 2 3
1
end_operator
begin_operator
block-write philosopher-0 forks--pid-wfork forks-0- queue-1 fork one
1
25 0
2
0 0 1 5
0 2 2 3
1
end_operator
begin_operator
block-write philosopher-0 forks-__-pidp1__2_-wfork forks-1- queue-1 fork one
1
26 0
2
0 0 3 7
0 3 2 3
1
end_operator
begin_operator
block-write philosopher-1 forks--pid-wfork forks-1- queue-1 fork one
1
26 0
2
0 1 1 5
0 3 2 3
1
end_operator
begin_operator
block-write philosopher-1 forks-__-pidp1__2_-wfork forks-0- queue-1 fork one
1
25 0
2
0 1 3 7
0 2 2 3
1
end_operator
begin_operator
perform-trans philosopher-0 philosopher forks--pid-rfork state-6 state-3
1
18 1
2
0 0 8 12
0 4 4 1
1
end_operator
begin_operator
perform-trans philosopher-0 philosopher forks--pid-wfork state-1 state-6
1
18 1
2
0 0 9 12
0 4 0 4
1
end_operator
begin_operator
perform-trans philosopher-0 philosopher forks--pid-wfork state-4 state-5
1
18 1
2
0 0 9 12
0 4 2 3
1
end_operator
begin_operator
perform-trans philosopher-0 philosopher forks-__-pidp1__2_-rfork state-3 state-4
1
18 1
2
0 0 10 12
0 4 1 2
1
end_operator
begin_operator
perform-trans philosopher-0 philosopher forks-__-pidp1__2_-wfork state-5 state-6
1
18 1
2
0 0 11 12
0 4 3 4
1
end_operator
begin_operator
perform-trans philosopher-1 philosopher forks--pid-rfork state-6 state-3
1
18 1
2
0 1 8 12
0 5 4 1
1
end_operator
begin_operator
perform-trans philosopher-1 philosopher forks--pid-wfork state-1 state-6
1
18 1
2
0 1 9 12
0 5 0 4
1
end_operator
begin_operator
perform-trans philosopher-1 philosopher forks--pid-wfork state-4 state-5
1
18 1
2
0 1 9 12
0 5 2 3
1
end_operator
begin_operator
perform-trans philosopher-1 philosopher forks-__-pidp1__2_-rfork state-3 state-4
1
18 1
2
0 1 10 12
0 5 1 2
1
end_operator
begin_operator
perform-trans philosopher-1 philosopher forks-__-pidp1__2_-wfork state-5 state-6
1
18 1
2
0 1 11 12
0 5 3 4
1
end_operator
begin_operator
queue-read philosopher-0 forks--pid-rfork forks-0- fork
1
20 0
8
0 0 0 8
1 1 4 1 -1 13
1 1 5 1 -1 13
1 1 6 1 -1 13
1 1 7 1 -1 13
0 2 2 0
0 6 -1 1
0 7 -1 1
1
end_operator
begin_operator
queue-read philosopher-0 forks-__-pidp1__2_-rfork forks-1- fork
1
22 0
8
0 0 2 10
1 1 4 1 -1 13
1 1 5 1 -1 13
1 1 6 1 -1 13
1 1 7 1 -1 13
0 3 2 0
0 6 -1 1
0 7 -1 1
1
end_operator
begin_operator
queue-read philosopher-1 forks--pid-rfork forks-1- fork
1
22 0
8
1 0 4 0 -1 13
1 0 5 0 -1 13
1 0 6 0 -1 13
1 0 7 0 -1 13
0 1 0 8
0 3 2 0
0 6 -1 1
0 7 -1 1
1
end_operator
begin_operator
queue-read philosopher-1 forks-__-pidp1__2_-rfork forks-0- fork
1
20 0
8
1 0 4 0 -1 13
1 0 5 0 -1 13
1 0 6 0 -1 13
1 0 7 0 -1 13
0 1 2 10
0 2 2 0
0 6 -1 1
0 7 -1 1
1
end_operator
begin_operator
queue-write philosopher-0 forks--pid-wfork forks-0- fork
0
9
0 0 1 9
1 1 4 1 -1 13
1 1 5 1 -1 13
1 1 6 1 -1 13
1 1 7 1 -1 13
0 2 2 1
0 6 -1 1
0 7 -1 1
0 27 -1 0
1
end_operator
begin_operator
queue-write philosopher-0 forks-__-pidp1__2_-wfork forks-1- fork
0
9
0 0 3 11
1 1 4 1 -1 13
1 1 5 1 -1 13
1 1 6 1 -1 13
1 1 7 1 -1 13
0 3 2 1
0 6 -1 1
0 7 -1 1
0 28 -1 0
1
end_operator
begin_operator
queue-write philosopher-1 forks--pid-wfork forks-1- fork
0
9
1 0 4 0 -1 13
1 0 5 0 -1 13
1 0 6 0 -1 13
1 0 7 0 -1 13
0 1 1 9
0 3 2 1
0 6 -1 1
0 7 -1 1
0 28 -1 0
1
end_operator
begin_operator
queue-write philosopher-1 forks-__-pidp1__2_-wfork forks-0- fork
0
9
1 0 4 0 -1 13
1 0 5 0 -1 13
1 0 6 0 -1 13
1 0 7 0 -1 13
0 1 3 11
0 2 2 1
0 6 -1 1
0 7 -1 1
0 27 -1 0
1
end_operator
11
begin_rule
1
0 4
12 0 1
end_rule
begin_rule
1
0 5
8 0 1
end_rule
begin_rule
1
0 5
10 0 1
end_rule
begin_rule
1
0 6
9 0 1
end_rule
begin_rule
1
0 7
11 0 1
end_rule
begin_rule
1
1 4
17 0 1
end_rule
begin_rule
1
1 5
13 0 1
end_rule
begin_rule
1
1 5
15 0 1
end_rule
begin_rule
1
1 6
14 0 1
end_rule
begin_rule
1
1 7
16 0 1
end_rule
begin_rule
2
2 2
3 2
18 0 1
end_rule
