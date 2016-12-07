#include <plasp/pddl/expressions/And.h>

#include <algorithm>
#include <iostream>

#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/expressions/Or.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// And
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string And::Identifier = "and";

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer And::decomposed(DerivedPredicates &derivedPredicates)
{
	derivedPredicates.emplace_back(new DerivedPredicate());
	auto &derivedPredicate = derivedPredicates.back();

	for (auto &argument : m_arguments)
		argument = argument->decomposed(derivedPredicates);

	// Move this expression’s arguments to the derived predicate
	derivedPredicate->setPreconditions({m_arguments});

	return derivedPredicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
