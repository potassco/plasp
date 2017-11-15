; tests that “imply” statements in preconditions are correctly reduced
(define (domain test-normalization)
	(:predicates
		(test-predicate-0))
)

(define (problem test-normalization)
	(:domain test-normalization)

	(:init
		(test-predicate-0))

	(:goal
		(test-predicate-0)
		(error)))
