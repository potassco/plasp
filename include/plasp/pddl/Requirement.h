#ifndef __PLASP__PDDL__REQUIREMENT_H
#define __PLASP__PDDL__REQUIREMENT_H

#include <plasp/utils/Parser.h>
#include <vector>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Requirement
{
	enum class Type
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
		ObjectFluents,
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

	using Types = std::vector<Type>;

	static Requirement::Type fromPDDL(utils::Parser &parser);

	static void toPDDL(std::ostream &ostream, Requirement::Type requirementType);
	static void toASP(std::ostream &ostream, Requirement::Type requirementType);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
