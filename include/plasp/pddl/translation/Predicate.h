#ifndef __PLASP__PDDL__TRANSLATION__PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__PREDICATE_H

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

#include <plasp/output/Formatting.h>

#include <plasp/pddl/translation/Primitives.h>
#include <plasp/pddl/translation/Variables.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void translatePredicate(output::ColorStream &outputStream, const ::pddl::ast::Predicate &predicate);
void translatePredicateDeclaration(output::ColorStream &outputStream, const ::pddl::ast::PredicateDeclaration &predicateDeclaration);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicate(output::ColorStream &outputStream, const ::pddl::ast::Predicate &predicate)
{
	const auto &arguments = predicate.arguments;

	if (arguments.empty())
	{
		outputStream << *predicate.declaration;
		return;
	}

	outputStream << "(" << *predicate.declaration;

	for (const auto &argument : arguments)
	{
		outputStream << ", ";

		const auto handleConstant =
			[&](const ::pddl::ast::ConstantPointer &constant)
			{
				outputStream << output::Keyword("constant") << "(" << *constant << ")";
			};

		const auto handleVariable =
			[&](const ::pddl::ast::VariablePointer &variable)
			{
				outputStream << *variable;
			};

		const auto handleUnsupported =
			[&](const auto &)
			{
				throw output::TranslatorException("only variables and constants supported in predicates currently");
			};

		argument.match(handleConstant, handleVariable, handleUnsupported);
	}

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicateDeclaration(output::ColorStream &outputStream, const ::pddl::ast::PredicateDeclaration &predicateDeclaration)
{
	outputStream << output::Keyword("variable") << "(";

	if (predicateDeclaration.parameters.empty())
	{
		outputStream << predicateDeclaration << ")";
		return;
	}

	outputStream << "(" << predicateDeclaration;
	translateVariablesForRuleHead(outputStream, predicateDeclaration.parameters);
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void translatePredicateToVariable(output::ColorStream &outputStream, const ::pddl::ast::Predicate &predicate, bool isPositive = true)
{
	outputStream << output::Keyword("variable") << "(";
	translatePredicate(outputStream, predicate);
	outputStream << "), " << output::Keyword("value") << "(";
	translatePredicate(outputStream, predicate);
	outputStream << ", ";

	if (isPositive)
		outputStream << output::Boolean("true");
	else
		outputStream << output::Boolean("false");

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
