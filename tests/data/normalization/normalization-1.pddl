; tests that a simple PDDL description is preserved by normalization
(define (domain test-normalization)
	(:requirements :typing)
	(:types block gripper)

	(:predicates
		(test-predicate-0)
		(test-predicate-1 ?x - block)
		(test-predicate-2 ?x - block ?y - gripper))

	(:action test-action-1
		:parameters
			(?x - block)
		:precondition
			(test-predicate-1 ?x)
		:effect
			(test-predicate-1 ?x))

	(:action test-action-2
		:parameters
			(?x - block ?y - gripper)
		:precondition
			(and
				(not (test-predicate-0))
				(test-predicate-2 ?x ?y))
		:effect
			(test-predicate-2 ?x ?y))
)

(define (problem test-normalization)
	(:domain test-normalization)

	(:objects a b c d - block e f - gripper)

	(:init
		(test-predicate-0)
		(test-predicate-1 a)
		(test-predicate-1 d)
		(test-predicate-2 a e)
		(test-predicate-2 c f))

	(:goal (and
		(test-predicate-0)
		(not (test-predicate-1 d))
		(test-predicate-2 b f))))
