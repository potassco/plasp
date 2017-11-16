; tests derived predicates are correclty numbered
; variation 1: derived predicates both in domain and problem
(define (domain test-normalization)
	(:predicates
		(test-predicate-0))

	; introduces derived predicates 1 and 2
	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(or
				(test-predicate-0)
				(and
					(test-predicate-0)
					(test-predicate-0)))
		:effect
			(test-predicate-0))

	; introduces derived predicates 3 and 4
	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(or
				(test-predicate-0)
				(and
					(test-predicate-0)
					(test-predicate-0)))
		:effect
			(test-predicate-0)))

(define (problem test-normalization)
	(:domain test-normalization)

	(:objects a b c)

	(:init
		(test-predicate-0))

	; introduces derived predicates 5 and 6
	(:goal
		(or
			(test-predicate-0)
			(and
				(test-predicate-0)
				(test-predicate-0)))))
