#ifndef __PLASP__PDDL__TRANSLATION__PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__PREDICATE_H

#include <plasp/output/Formatting.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/DerivedPredicate.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/Variable.h>
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

// TODO: refactor (overly redundant)
void translatePredicate(output::ColorStream &outputStream, const expressions::Predicate &predicate);
void translateDerivedPredicate(output::ColorStream &outputStream, const expressions::DerivedPredicate &derivedPredicate);
void printPredicateName(output::ColorStream &outputStream, const expressions::PredicateDeclaration &predicateDeclaration);
void printDerivedPredicateName(output::ColorStream &outputStream, const expressions::DerivedPredicate &derivedPredicate);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicate(output::ColorStream &outputStream, const expressions::Predicate &predicate)
{
	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		outputStream << output::String(predicate.name().c_str());

		return;
	}

	outputStream << "(" << output::String(predicate.name().c_str());

	for (const auto &argument : arguments)
	{
		outputStream << ", ";

		if (argument->is<expressions::Constant>())
		{
			const auto &constant = argument->as<expressions::Constant>();

			outputStream << output::Keyword("constant") << "(" << output::String(constant.name().c_str()) << ")";
		}
		else if (argument->is<expressions::Variable>())
		{
			const auto &variable = argument->as<expressions::Variable>();

			outputStream << output::Variable(variable.name().c_str());
		}
		else
			throw output::TranslatorException("only variables and constants supported in predicates currently");
	}

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicate(output::ColorStream &outputStream, const expressions::DerivedPredicate &derivedPredicate)
{
	const auto &parameters = derivedPredicate.parameters();
	const auto id = derivedPredicate.id();

	if (parameters.empty())
	{
		outputStream << output::Number<decltype(id)>(id);
		return;
	}

	outputStream << "(" << output::Number<decltype(id)>(id);

	for (const auto &parameter : parameters)
		outputStream << ", " << output::Variable(parameter->name().c_str());

	outputStream << ")";
}

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
