; tests that “imply” statements in preconditions are correctly reduced
(define (domain test-normalization)
	(:predicates
		(test-predicate-0)
		(test-predicate-1 ?x)
		(test-predicate-2 ?x ?y))

	; top-level “imply” statement
	(:action test-action-1
		:parameters
			(?x)
		:precondition
			(imply
				(test-predicate-0)
				(test-predicate-1 ?x))
		:effect
			(test-predicate-1 ?x))

	; nested “imply” statement
	(:action test-action-2
		:parameters
			(?x)
		:precondition
			(and
				(and
					(imply
						(test-predicate-0)
						(test-predicate-1 ?x))
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

	; “imply” statement in goal
	(:goal
		(and
			(imply
				(test-predicate-0)
				(test-predicate-1 a))
			(test-predicate-0))))
