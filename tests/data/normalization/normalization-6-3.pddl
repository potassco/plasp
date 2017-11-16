; tests derived predicates are correclty numbered
; variation 3: derived predicates in problem only
(define (domain test-normalization)
	(:predicates
		(test-predicate-0))

	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(test-predicate-0)
		:effect
			(test-predicate-0)))

(define (problem test-normalization)
	(:domain test-normalization)

	(:objects a b c)

	(:init
		(test-predicate-0))

	; introduces derived predicates 1 and 2
	(:goal
		(or
			(test-predicate-0)
			(and
				(test-predicate-0)
				(test-predicate-0)))))
