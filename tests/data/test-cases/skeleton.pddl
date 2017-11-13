(define
	(domain skeleton)
	(:requirements :typing)
	(:types
		a1 a2 a3 - object
		b1 - a1
		b2 - a2
		b3 - a3
		bx - a1
		bx - a2
		bx - a3
		c1 - b1
		c2 - b2
		c3 - b3
		cx - c1
		cx - c2
		cx - c3
	)
	(:predicates
		(p1 ?x - a1 ?y - a2 ?z - a3)
		(p2 ?x - a1 ?y - b2 ?z - c3)
		(p3 ?x - (either a1 a2) ?y ?z - (either b1 b3))
	)
	(:constants
		ca1 - a1
		ca2 - a2
		ca3 - a3
		cb1 - b1
		cb2 - b2
		cb3 - b3
		cbx - bx
		cc1 - c1
		cc2 - c2
		cc3 - c3
		ccx - cx
	)
