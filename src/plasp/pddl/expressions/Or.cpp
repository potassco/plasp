#include <plasp/pddl/expressions/Or.h>

#include <plasp/pddl/expressions/DerivedPredicate.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Or
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string Or::Identifier = "or";

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Or::decomposed(DerivedPredicates &derivedPredicates)
{
	// Check that all children are simple or negated predicates
	std::for_each(m_arguments.begin(), m_arguments.end(),
		[&](auto &argument)
		{
			argument = argument->decomposed(derivedPredicates);
		});

	auto derivedPredicate = DerivedPredicatePointer(new DerivedPredicate(derivedPredicates.size()));
	derivedPredicates.push_back(derivedPredicate);

	derivedPredicate->setArgument(this);

	return derivedPredicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
