
(define (domain openstacks-netbenefit-numeric-ADL)
  (:requirements :typing :adl :numeric-fluents :goal-utilities)
  (:types order product)
  (:predicates (includes ?o - order ?p - product)
	       (waiting ?o - order)
	       (started ?o - order)
	       (shipped ?o - order)
	       (delivered ?o - order ?p - product)
	       (made ?p - product))

	       
  (:action open-new-stack
    :parameters ()
    :precondition (and)
    )
	      
   (:action start-order
    :parameters (?o - order)
    :precondition (waiting ?o)
    )
    
  (:action make-product
    :parameters (?p - product)
    :precondition (and (not (made ?p)))
    )
    
  (:action ship-order
    :parameters (?o - order)
    :precondition (and (started ?o))
    )

  )
