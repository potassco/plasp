(define (domain test-normalization)
	(:predicates
		(p1 ?x ?y)
		(p2 ?x ?y)
		(p3 ?x ?y)
		(p4 ?x ?y)
		(p5 ?x ?y ?z)
		(p6 ?x ?y ?z)
		(p7 ?x ?y)
		(p8 ?x)
		(p9 ?x))

	(:action test-action-1
		:parameters
			(?x
			?y)
		:precondition
			(and
				(forall
					(?z)
					(imply
						(p1 ?y ?z)
						(p2 ?z ?y)))
				(p3 ?x ?y)
				(or
					(and
						(p4 ?x ?y)
						(forall
							(?z)
							(imply
								(p5 ?z ?x ?y)
								(p3 ?z ?y))))
					(and
						(p7 ?x ?y)
						(not (exists
							(?z ?a)
							(and
								(p6 ?z ?a ?y)
								(p3 ?z ?y)))))))
		:effect
			(and
				(not (p3 ?x ?y))
				(p9 ?x)
				(when
					(and
						(not (forall
							(?p)
							(and
								(not (p7 ?p ?y))
								(not (p3 ?y ?p)))))
						(exists
							(?z ?a)
							(or
								(p4 ?z ?a)
								(p3 ?a ?z))))
					(and
						(p8 ?y)
						(p9 ?y))))))
