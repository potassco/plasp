#include <plasp/pddl/expressions/Or.h>

#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/expressions/And.h>
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
	derivedPredicates.emplace_back(new DerivedPredicate());
	auto &derivedPredicate = derivedPredicates.back();

	std::vector<Expressions> preconditions;

	for (auto &argument : m_arguments)
	{
		Expressions conjunction;

		// “and” expressions can directly be inlined into the derived predicate
		if (argument->is<expressions::And>())
		{
			const auto &andExpression = dynamic_cast<expressions::And &>(*argument);

			conjunction = std::move(andExpression.arguments());

			for (auto &argument : conjunction)
				argument = argument->decomposed(derivedPredicates);

			break;
		}
		else
		{
			conjunction.emplace_back(argument->decomposed(derivedPredicates));
			break;
		}

		// Move this expression’s arguments to the derived predicate
		preconditions.emplace_back(std::move(conjunction));
	}

	derivedPredicate->setPreconditions(std::move(preconditions));

	return derivedPredicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
