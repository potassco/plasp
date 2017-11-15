; tests that universal quantifiers in preconditions are correctly reduced
(define (domain test-normalization)
	(:predicates
		(test-predicate-0)
		(test-predicate-1 ?x)
		(test-predicate-2 ?x ?y))

	; top-level “forall” statement
	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(forall
				(?x ?y)
				(test-predicate-2 ?x ?y))
		:effect
			(test-predicate-1 ?x))

	; nested “forall” statement
	(:action test-action-2
		:parameters
			(?x)
		:precondition
			(and
				(and
					(forall
						(?x ?y)
						(test-predicate-2 ?x ?y))
					(test-predicate-0))
				(test-predicate-0))
		:effect
			(test-predicate-1 ?x))
)

(define (problem test-normalization)
	(:domain test-normalization)

	(:objects a b c)

	(:init
		(test-predicate-0))

	; “forall” statement in goal
	(:goal
		(and
			(forall
				(?x ?y)
				(test-predicate-2 ?x ?y))
			(test-predicate-0))))
