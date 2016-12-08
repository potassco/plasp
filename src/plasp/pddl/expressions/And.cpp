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
	BOOST_ASSERT(!m_arguments.empty());

	derivedPredicates.emplace_back(new DerivedPredicate(derivedPredicates.size()));
	auto derivedPredicate = derivedPredicates.back();

	for (auto &argument : m_arguments)
		argument = argument->decomposed(derivedPredicates);

	std::vector<Expressions> preconditions;
	preconditions.emplace_back(std::move(m_arguments));

	// Move this expression’s arguments to the derived predicate
	derivedPredicate->setPreconditions(std::move(preconditions));

	return derivedPredicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
