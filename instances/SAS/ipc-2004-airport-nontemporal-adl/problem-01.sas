begin_version
3
end_version
begin_metric
0
end_metric
34
begin_variable
var0
-1
15
Atom airborne(airplane_cfbeg, seg_rwe_0_50)
Atom airborne(airplane_cfbeg, seg_rww_0_50)
Atom at-segment(airplane_cfbeg, seg_pp_0_60)
Atom at-segment(airplane_cfbeg, seg_ppdoor_0_40)
Atom at-segment(airplane_cfbeg, seg_rw_0_400)
Atom at-segment(airplane_cfbeg, seg_rwe_0_50)
Atom at-segment(airplane_cfbeg, seg_rww_0_50)
Atom at-segment(airplane_cfbeg, seg_twe1_0_200)
Atom at-segment(airplane_cfbeg, seg_twe2_0_50)
Atom at-segment(airplane_cfbeg, seg_twe3_0_50)
Atom at-segment(airplane_cfbeg, seg_twe4_0_50)
Atom at-segment(airplane_cfbeg, seg_tww1_0_200)
Atom at-segment(airplane_cfbeg, seg_tww2_0_50)
Atom at-segment(airplane_cfbeg, seg_tww3_0_50)
Atom at-segment(airplane_cfbeg, seg_tww4_0_50)
end_variable
begin_variable
var1
-1
2
Atom blocked(seg_pp_0_60, airplane_cfbeg)
NegatedAtom blocked(seg_pp_0_60, airplane_cfbeg)
end_variable
begin_variable
var2
-1
2
Atom blocked(seg_ppdoor_0_40, airplane_cfbeg)
NegatedAtom blocked(seg_ppdoor_0_40, airplane_cfbeg)
end_variable
begin_variable
var3
-1
2
Atom blocked(seg_rw_0_400, airplane_cfbeg)
NegatedAtom blocked(seg_rw_0_400, airplane_cfbeg)
end_variable
begin_variable
var4
-1
2
Atom blocked(seg_rwe_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_rwe_0_50, airplane_cfbeg)
end_variable
begin_variable
var5
-1
2
Atom blocked(seg_rwtw1_0_10, airplane_cfbeg)
NegatedAtom blocked(seg_rwtw1_0_10, airplane_cfbeg)
end_variable
begin_variable
var6
-1
2
Atom blocked(seg_rww_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_rww_0_50, airplane_cfbeg)
end_variable
begin_variable
var7
-1
2
Atom blocked(seg_twe1_0_200, airplane_cfbeg)
NegatedAtom blocked(seg_twe1_0_200, airplane_cfbeg)
end_variable
begin_variable
var8
-1
2
Atom blocked(seg_twe2_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_twe2_0_50, airplane_cfbeg)
end_variable
begin_variable
var9
-1
2
Atom blocked(seg_twe3_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_twe3_0_50, airplane_cfbeg)
end_variable
begin_variable
var10
-1
2
Atom blocked(seg_twe4_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_twe4_0_50, airplane_cfbeg)
end_variable
begin_variable
var11
-1
2
Atom blocked(seg_tww1_0_200, airplane_cfbeg)
NegatedAtom blocked(seg_tww1_0_200, airplane_cfbeg)
end_variable
begin_variable
var12
-1
2
Atom blocked(seg_tww2_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_tww2_0_50, airplane_cfbeg)
end_variable
begin_variable
var13
-1
2
Atom blocked(seg_tww3_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_tww3_0_50, airplane_cfbeg)
end_variable
begin_variable
var14
-1
2
Atom blocked(seg_tww4_0_50, airplane_cfbeg)
NegatedAtom blocked(seg_tww4_0_50, airplane_cfbeg)
end_variable
begin_variable
var15
-1
2
Atom facing(airplane_cfbeg, north)
Atom facing(airplane_cfbeg, south)
end_variable
begin_variable
var16
-1
14
Atom is-moving(airplane_cfbeg)
Atom is-parked(airplane_cfbeg, seg_pp_0_60)
Atom is-parked(airplane_cfbeg, seg_ppdoor_0_40)
Atom is-parked(airplane_cfbeg, seg_rw_0_400)
Atom is-parked(airplane_cfbeg, seg_rwe_0_50)
Atom is-parked(airplane_cfbeg, seg_rww_0_50)
Atom is-parked(airplane_cfbeg, seg_twe1_0_200)
Atom is-parked(airplane_cfbeg, seg_twe2_0_50)
Atom is-parked(airplane_cfbeg, seg_twe3_0_50)
Atom is-parked(airplane_cfbeg, seg_twe4_0_50)
Atom is-parked(airplane_cfbeg, seg_tww1_0_200)
Atom is-parked(airplane_cfbeg, seg_tww2_0_50)
Atom is-parked(airplane_cfbeg, seg_tww3_0_50)
Atom is-parked(airplane_cfbeg, seg_tww4_0_50)
end_variable
begin_variable
var17
1
2
Atom new-axiom@1(south, seg_ppdoor_0_40, seg_tww1_0_200, medium)
NegatedAtom new-axiom@1(south, seg_ppdoor_0_40, seg_tww1_0_200, medium)
end_variable
begin_variable
var18
1
2
Atom new-axiom@1(south, seg_rw_0_400, seg_rww_0_50, medium)
NegatedAtom new-axiom@1(south, seg_rw_0_400, seg_rww_0_50, medium)
end_variable
begin_variable
var19
1
2
Atom new-axiom@1(south, seg_tww1_0_200, seg_ppdoor_0_40, medium)
NegatedAtom new-axiom@1(south, seg_tww1_0_200, seg_ppdoor_0_40, medium)
end_variable
begin_variable
var20
1
2
Atom new-axiom@1(south, seg_tww1_0_200, seg_twe1_0_200, medium)
NegatedAtom new-axiom@1(south, seg_tww1_0_200, seg_twe1_0_200, medium)
end_variable
begin_variable
var21
-1
2
Atom occupied(seg_pp_0_60)
NegatedAtom occupied(seg_pp_0_60)
end_variable
begin_variable
var22
-1
2
Atom occupied(seg_ppdoor_0_40)
NegatedAtom occupied(seg_ppdoor_0_40)
end_variable
begin_variable
var23
-1
2
Atom occupied(seg_rw_0_400)
NegatedAtom occupied(seg_rw_0_400)
end_variable
begin_variable
var24
-1
2
Atom occupied(seg_rwe_0_50)
NegatedAtom occupied(seg_rwe_0_50)
end_variable
begin_variable
var25
-1
2
Atom occupied(seg_rww_0_50)
NegatedAtom occupied(seg_rww_0_50)
end_variable
begin_variable
var26
-1
2
Atom occupied(seg_twe1_0_200)
NegatedAtom occupied(seg_twe1_0_200)
end_variable
begin_variable
var27
-1
2
Atom occupied(seg_twe2_0_50)
NegatedAtom occupied(seg_twe2_0_50)
end_variable
begin_variable
var28
-1
2
Atom occupied(seg_twe3_0_50)
NegatedAtom occupied(seg_twe3_0_50)
end_variable
begin_variable
var29
-1
2
Atom occupied(seg_twe4_0_50)
NegatedAtom occupied(seg_twe4_0_50)
end_variable
begin_variable
var30
-1
2
Atom occupied(seg_tww1_0_200)
NegatedAtom occupied(seg_tww1_0_200)
end_variable
begin_variable
var31
-1
2
Atom occupied(seg_tww2_0_50)
NegatedAtom occupied(seg_tww2_0_50)
end_variable
begin_variable
var32
-1
2
Atom occupied(seg_tww3_0_50)
NegatedAtom occupied(seg_tww3_0_50)
end_variable
begin_variable
var33
-1
2
Atom occupied(seg_tww4_0_50)
NegatedAtom occupied(seg_tww4_0_50)
end_variable
4
begin_mutex_group
15
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
0 13
0 14
end_mutex_group
begin_mutex_group
13
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
0 13
0 14
end_mutex_group
begin_mutex_group
2
15 0
15 1
end_mutex_group
begin_mutex_group
14
16 0
16 1
16 2
16 3
16 4
16 5
16 6
16 7
16 8
16 9
16 10
16 11
16 12
16 13
end_mutex_group
begin_state
4
1
1
0
0
1
1
1
1
1
1
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
1
1
0
1
1
1
1
1
1
1
1
1
1
end_state
begin_goal
1
16 1
end_goal
43
begin_operator
move airplane_cfbeg medium north seg_pp_0_60 seg_ppdoor_0_40 north
2
15 0
16 0
5
0 0 2 3
0 1 -1 0
0 2 -1 0
0 21 -1 1
0 22 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_ppdoor_0_40 seg_tww1_0_200 south
2
16 0
17 1
8
0 0 3 11
0 1 -1 1
0 2 -1 0
0 7 -1 0
0 11 -1 0
0 15 0 1
0 22 -1 1
0 30 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_tww1_0_200 seg_ppdoor_0_40 south
2
16 0
19 1
8
0 0 11 3
0 2 -1 0
0 7 -1 0
0 11 -1 0
0 12 -1 1
0 15 0 1
0 22 -1 0
0 30 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_tww1_0_200 seg_twe1_0_200 south
2
16 0
20 1
8
0 0 11 7
0 2 -1 0
0 7 -1 0
0 11 -1 0
0 12 -1 1
0 15 0 1
0 26 -1 0
0 30 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_tww2_0_50 seg_tww1_0_200 north
2
15 0
16 0
6
0 0 12 11
0 11 -1 0
0 12 -1 0
0 13 -1 1
0 30 -1 0
0 31 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_tww3_0_50 seg_tww2_0_50 north
2
15 0
16 0
6
0 0 13 12
0 12 -1 0
0 13 -1 0
0 14 -1 1
0 31 -1 0
0 32 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium north seg_tww4_0_50 seg_tww3_0_50 north
2
15 0
16 0
7
0 0 14 13
0 5 -1 1
0 6 -1 1
0 13 -1 0
0 14 -1 0
0 32 -1 0
0 33 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_ppdoor_0_40 seg_pp_0_60 south
2
15 1
16 0
7
0 0 3 2
0 1 -1 0
0 2 -1 0
0 7 -1 1
0 11 -1 1
0 21 -1 0
0 22 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_rw_0_400 seg_rww_0_50 south
3
15 1
16 0
18 1
6
0 0 4 6
0 3 -1 0
0 4 -1 0
0 6 -1 0
0 23 -1 1
0 25 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_rwe_0_50 seg_rw_0_400 south
2
15 1
16 0
6
0 0 5 4
0 3 -1 0
0 4 -1 0
0 10 -1 1
0 23 -1 0
0 24 -1 1
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_rww_0_50 seg_tww4_0_50 north
1
16 0
9
0 0 6 14
0 3 -1 1
0 4 -1 1
0 5 -1 0
0 6 -1 0
0 14 -1 0
0 15 1 0
0 25 -1 1
0 33 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_twe1_0_200 seg_twe2_0_50 south
2
15 1
16 0
7
0 0 7 8
0 2 -1 1
0 7 -1 0
0 8 -1 0
0 11 -1 1
0 26 -1 1
0 27 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_twe2_0_50 seg_twe3_0_50 south
2
15 1
16 0
6
0 0 8 9
0 7 -1 1
0 8 -1 0
0 9 -1 0
0 27 -1 1
0 28 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_twe3_0_50 seg_twe4_0_50 south
2
15 1
16 0
6
0 0 9 10
0 8 -1 1
0 9 -1 0
0 10 -1 0
0 28 -1 1
0 29 -1 0
1
end_operator
begin_operator
move airplane_cfbeg medium south seg_twe4_0_50 seg_rwe_0_50 south
2
15 1
16 0
6
0 0 10 5
0 4 -1 0
0 9 -1 1
0 10 -1 0
0 24 -1 0
0 29 -1 1
1
end_operator
begin_operator
park airplane_cfbeg medium seg_pp_0_60 north
2
0 2
15 0
1
0 16 0 1
1
end_operator
begin_operator
park airplane_cfbeg medium seg_pp_0_60 south
2
0 2
15 1
2
0 2 -1 1
0 16 0 1
1
end_operator
begin_operator
park airplane_cfbeg medium seg_ppdoor_0_40 north
2
0 3
15 0
2
0 1 -1 1
0 16 0 2
1
end_operator
begin_operator
park airplane_cfbeg medium seg_ppdoor_0_40 south
2
0 3
15 1
3
0 7 -1 1
0 11 -1 1
0 16 0 2
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rw_0_400 north
2
0 4
15 0
2
0 6 -1 1
0 16 0 3
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rw_0_400 south
2
0 4
15 1
2
0 4 -1 1
0 16 0 3
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rwe_0_50 north
2
0 5
15 0
3
0 3 -1 1
0 6 -1 1
0 16 0 4
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rwe_0_50 south
2
0 5
15 1
2
0 10 -1 1
0 16 0 4
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rww_0_50 north
2
0 6
15 0
2
0 14 -1 1
0 16 0 5
1
end_operator
begin_operator
park airplane_cfbeg medium seg_rww_0_50 south
2
0 6
15 1
3
0 3 -1 1
0 4 -1 1
0 16 0 5
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe1_0_200 north
2
0 7
15 0
2
0 8 -1 1
0 16 0 6
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe1_0_200 south
2
0 7
15 1
3
0 2 -1 1
0 11 -1 1
0 16 0 6
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe2_0_50 north
2
0 8
15 0
2
0 9 -1 1
0 16 0 7
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe2_0_50 south
2
0 8
15 1
2
0 7 -1 1
0 16 0 7
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe3_0_50 north
2
0 9
15 0
2
0 10 -1 1
0 16 0 8
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe3_0_50 south
2
0 9
15 1
2
0 8 -1 1
0 16 0 8
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe4_0_50 north
2
0 10
15 0
2
0 4 -1 1
0 16 0 9
1
end_operator
begin_operator
park airplane_cfbeg medium seg_twe4_0_50 south
2
0 10
15 1
2
0 9 -1 1
0 16 0 9
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww1_0_200 north
2
0 11
15 0
2
0 12 -1 1
0 16 0 10
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww1_0_200 south
2
0 11
15 1
3
0 2 -1 1
0 7 -1 1
0 16 0 10
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww2_0_50 north
2
0 12
15 0
2
0 13 -1 1
0 16 0 11
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww2_0_50 south
2
0 12
15 1
2
0 11 -1 1
0 16 0 11
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww3_0_50 north
2
0 13
15 0
2
0 14 -1 1
0 16 0 12
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww3_0_50 south
2
0 13
15 1
2
0 12 -1 1
0 16 0 12
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww4_0_50 north
2
0 14
15 0
3
0 5 -1 1
0 6 -1 1
0 16 0 13
1
end_operator
begin_operator
park airplane_cfbeg medium seg_tww4_0_50 south
2
0 14
15 1
2
0 13 -1 1
0 16 0 13
1
end_operator
begin_operator
takeoff airplane_cfbeg seg_rwe_0_50 south
1
15 1
16
0 0 5 0
0 1 -1 1
0 2 -1 1
0 3 -1 1
0 4 -1 1
0 5 -1 1
0 6 -1 1
0 7 -1 1
0 8 -1 1
0 9 -1 1
0 10 -1 1
0 11 -1 1
0 12 -1 1
0 13 -1 1
0 14 -1 1
0 24 -1 1
1
end_operator
begin_operator
takeoff airplane_cfbeg seg_rww_0_50 north
1
15 0
16
0 0 6 1
0 1 -1 1
0 2 -1 1
0 3 -1 1
0 4 -1 1
0 5 -1 1
0 6 -1 1
0 7 -1 1
0 8 -1 1
0 9 -1 1
0 10 -1 1
0 11 -1 1
0 12 -1 1
0 13 -1 1
0 14 -1 1
0 25 -1 1
1
end_operator
4
begin_rule
1
22 1
20 0 1
end_rule
begin_rule
1
24 1
18 0 1
end_rule
begin_rule
1
26 1
17 0 1
end_rule
begin_rule
1
26 1
19 0 1
end_rule
