#ifndef __PLASP__PDDL__TRANSLATION__PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__PREDICATE_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>
#include <pddl/Parse.h>

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

void translatePredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs);
void translatePredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::PredicateDeclaration &predicateDeclaration, VariableIDMap &variableIDs);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs)
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
			[&](const ::pddl::normalizedAST::ConstantPointer &constant)
			{
				outputStream << colorlog::Keyword("constant") << "(" << *constant << ")";
			};

		const auto handleVariable =
			[&](const ::pddl::normalizedAST::VariablePointer &variable)
			{
				translateVariable(outputStream, *variable, variableIDs);
			};

		argument.match(handleConstant, handleVariable);
	}

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::PredicateDeclaration &predicateDeclaration, VariableIDMap &variableIDs)
{
	outputStream << colorlog::Keyword("variable") << "(";

	if (predicateDeclaration.parameters.empty())
	{
		outputStream << predicateDeclaration << ")";
		return;
	}

	outputStream << "(" << predicateDeclaration;
	translateVariablesForRuleHead(outputStream, predicateDeclaration.parameters, variableIDs);
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void translatePredicateToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Predicate &predicate, VariableIDMap &variableIDs, bool isPositive = true)
{
	outputStream << colorlog::Keyword("variable") << "(";

	translatePredicate(outputStream, predicate, variableIDs);

	outputStream
		<< "), "
		<< colorlog::Keyword("value") << "("
		<< colorlog::Keyword("variable") << "(";

	translatePredicate(outputStream, predicate, variableIDs);

	outputStream << "), ";

	if (isPositive)
		outputStream << colorlog::Boolean("true");
	else
		outputStream << colorlog::Boolean("false");

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
