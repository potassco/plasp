#ifndef __PLASP__PDDL__REQUIREMENT_H
#define __PLASP__PDDL__REQUIREMENT_H

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class Requirement
{
	STRIPS,
	Typing,
	NegativePreconditions,
	DisjunctivePreconditions,
	Equality,
	ExistentialPreconditions,
	UniversalPreconditions,
	QuantifiedPreconditions,
	ConditionalEffects,
	Fluents,
	NumericFluents,
	ADL,
	DurativeActions,
	DurationInequalities,
	ContinuousEffects,
	DerivedPredicates,
	TimedInitialLiterals,
	Preferences,
	Constraints,
	ActionCosts
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
