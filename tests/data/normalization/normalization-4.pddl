; tests that negations are correctly normalized
(define (domain test-normalization)
	(:predicates
		(test-predicate-0)
		(test-predicate-1 ?x)
		(test-predicate-2 ?x ?y))

	; multiple negations
	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(not (not (not (not (not (not (test-predicate-1 ?x)))))))
		:effect
			(test-predicate-1 ?x))

	; multiple negations
	(:action test-action-2
		:parameters
			(?x)
		:precondition
			(not (not (not (not (not (test-predicate-1 ?x))))))
		:effect
			(test-predicate-1 ?x))

	; negated “exists” statement
	(:action test-action-3
		:parameters
			(?x)
		:precondition
			(not (exists
				(?x ?y)
				(not (not (test-predicate-2 ?x ?y)))))
		:effect
			(test-predicate-1 ?x))

	; negated “forall” statement
	(:action test-action-4
		:parameters
			(?x)
		:precondition
			(not (forall
				(?x ?y)
				(not (not (not (test-predicate-2 ?x ?y))))))
		:effect
			(test-predicate-1 ?x))

	; negations introduced by reduction
	(:action test-action-5
		:parameters
			(?x)
		:precondition
			(imply
				(not (test-predicate-0))
				(test-predicate-1 ?x))
		:effect
			(test-predicate-1 ?x))

	; negated disjunction
	(:action test-action-6
		:parameters
			(?x)
		:precondition
			(not (imply
				(test-predicate-0)
				(not (test-predicate-1 ?x))))
		:effect
			(test-predicate-1 ?x))

	; negated conjunction
	(:action test-action-7
		:parameters
			(?x)
		:precondition
			(not (and
				(not (test-predicate-0))
				(not (test-predicate-1 ?x))))
		:effect
			(test-predicate-1 ?x))
)
