(define (domain tsp)
(:requirements :negative-preconditions)
  (:predicates
     (at ?x)
     (visited ?x)
      (connected ?x ?y))

  (:action move
	:parameters (?x ?y)
	:precondition (and (at ?x) (not (visited ?y)) (connected ?x ?y))
	:effect (and (at ?y) (visited ?y) (not (at ?x)))))
