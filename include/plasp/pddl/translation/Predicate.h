#ifndef __PLASP__PDDL__TRANSLATION__PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__PREDICATE_H

#include <plasp/output/Formatting.h>
#include <plasp/pddl/expressions/DerivedPredicate.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/translation/Variables.h>

namespace plasp
{
namespace pddl
{
namespace translation
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void printPredicateName(output::ColorStream &outputStream, const expressions::PredicateDeclaration &predicateDeclaration);
void printDerivedPredicateName(output::ColorStream &outputStream, const expressions::DerivedPredicate &derivedPredicate);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void printPredicateName(output::ColorStream &outputStream, const expressions::PredicateDeclaration &predicateDeclaration)
{
	outputStream << output::Keyword("variable") << "(";

	if (predicateDeclaration.parameters().empty())
	{
		outputStream << output::String(predicateDeclaration.name().c_str()) << ")";
		return;
	}

	outputStream << "(" << output::String(predicateDeclaration.name().c_str());
	translation::translateVariablesForRuleHead(outputStream, predicateDeclaration.parameters());
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void printDerivedPredicateName(output::ColorStream &outputStream, const expressions::DerivedPredicate &derivedPredicate)
{
	outputStream << output::Keyword("derivedVariable") << "(";

	const auto id = derivedPredicate.id();

	if (derivedPredicate.parameters().empty())
	{
		outputStream << output::Number<decltype(id)>(id) << ")";
		return;
	}

	outputStream << "(" << output::Number<decltype(id)>(id);
	translation::translateVariablesForRuleHead(outputStream, derivedPredicate.parameters());
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
