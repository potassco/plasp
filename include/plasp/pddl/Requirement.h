#ifndef __PLASP__PDDL__REQUIREMENT_H
#define __PLASP__PDDL__REQUIREMENT_H

#include <vector>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Requirement;
using Requirements = std::vector<Requirement>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Requirement
{
	public:
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

		static Requirement parse(utils::Parser &parser);

	public:
		Requirement(Type type);

		Type type() const;

		void printAsPDDL(std::ostream &ostream) const;
		void printAsASP(std::ostream &ostream) const;

	private:
		Type m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
