(define (domain logistics-strips)
  (:requirements :strips) 
  (:predicates 	(OBJ ?obj)
	       	(TRUCK ?truck)
               	(LOCATION ?loc)
		(AIRPLANE ?airplane)
                (CITY ?city)
                (AIRPORT ?airport)
		(at ?obj ?loc)
		(in ?obj1 ?obj2)
		(in-city ?obj ?city))
 
  ; (:types )		; default object

(:action LOAD-TRUCK
  :parameters
   (?obj
    ?truck
    ?loc)
  :precondition
   (and (OBJ ?obj) (TRUCK ?truck) (LOCATION ?loc)
   (at ?truck ?loc) (at ?obj ?loc))
  :effect
   (and (not (at ?obj ?loc)) (in ?obj ?truck)))

(:action LOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
   (at ?obj ?loc) (at ?airplane ?loc))
  :effect
   (and (not (at ?obj ?loc)) (in ?obj ?airplane)))

(:action UNLOAD-TRUCK
  :parameters
   (?obj
    ?truck
    ?loc)
  :precondition
   (and (OBJ ?obj) (TRUCK ?truck) (LOCATION ?loc)
        (at ?truck ?loc) (in ?obj ?truck))
  :effect
   (and (not (in ?obj ?truck)) (at ?obj ?loc)))

(:action UNLOAD-AIRPLANE
  :parameters
   (?obj
    ?airplane
    ?loc)
  :precondition
   (and (OBJ ?obj) (AIRPLANE ?airplane) (LOCATION ?loc)
        (in ?obj ?airplane) (at ?airplane ?loc))
  :effect
   (and (not (in ?obj ?airplane)) (at ?obj ?loc)))

(:action DRIVE-TRUCK
  :parameters
   (?truck
    ?loc-from
    ?loc-to
    ?city)
  :precondition
   (and (TRUCK ?truck) (LOCATION ?loc-from) (LOCATION ?loc-to) (CITY ?city)
   (at ?truck ?loc-from)
   (in-city ?loc-from ?city)
   (in-city ?loc-to ?city))
  :effect
   (and (not (at ?truck ?loc-from)) (at ?truck ?loc-to)))

(:action FLY-AIRPLANE
  :parameters
   (?airplane
    ?loc-from
    ?loc-to)
  :precondition
   (and (AIRPLANE ?airplane) (AIRPORT ?loc-from) (AIRPORT ?loc-to)
	(at ?airplane ?loc-from))
  :effect
   (and (not (at ?airplane ?loc-from)) (at ?airplane ?loc-to)))
)
(define (problem strips-log-x-1)
   (:domain logistics-strips)
   (:objects package6 package5 package4 package3 package2 package1
             city6 city5 city4 city3 city2 city1 truck6 truck5 truck4
             truck3 truck2 truck1 plane2 plane1 city6-1 city5-1 city4-1
             city3-1 city2-1 city1-1 city6-2 city5-2 city4-2 city3-2
             city2-2 city1-2)
   (:init (obj package6)
          (obj package5)
          (obj package4)
          (obj package3)
          (obj package2)
          (obj package1)
          (city city6)
          (city city5)
          (city city4)
          (city city3)
          (city city2)
          (city city1)
          (truck truck6)
          (truck truck5)
          (truck truck4)
          (truck truck3)
          (truck truck2)
          (truck truck1)
          (airplane plane2)
          (airplane plane1)
          (location city6-1)
          (location city5-1)
          (location city4-1)
          (location city3-1)
          (location city2-1)
          (location city1-1)
          (airport city6-2)
          (location city6-2)
          (airport city5-2)
          (location city5-2)
          (airport city4-2)
          (location city4-2)
          (airport city3-2)
          (location city3-2)
          (airport city2-2)
          (location city2-2)
          (airport city1-2)
          (location city1-2)
          (in-city city6-2 city6)
          (in-city city6-1 city6)
          (in-city city5-2 city5)
          (in-city city5-1 city5)
          (in-city city4-2 city4)
          (in-city city4-1 city4)
          (in-city city3-2 city3)
          (in-city city3-1 city3)
          (in-city city2-2 city2)
          (in-city city2-1 city2)
          (in-city city1-2 city1)
          (in-city city1-1 city1)
          (at plane2 city4-2)
          (at plane1 city4-2)
          (at truck6 city6-1)
          (at truck5 city5-1)
          (at truck4 city4-1)
          (at truck3 city3-1)
          (at truck2 city2-1)
          (at truck1 city1-1)
          (at package6 city3-1)
          (at package5 city4-2)
          (at package4 city1-1)
          (at package3 city1-1)
          (at package2 city1-2)
          (at package1 city2-1))
   (:goal (and (at package6 city1-2)
               (at package5 city6-2)
               (at package4 city3-2)
               (at package3 city6-1)
               (at package2 city6-2)
               (at package1 city2-1))))
