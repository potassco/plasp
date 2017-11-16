; tests derived predicates are correclty numbered
; variation 2: derived predicates in domain only
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
			(test-predicate-0))
)
