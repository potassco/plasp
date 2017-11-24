#ifndef __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_H

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
// DerivedPredicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void translateDerivedPredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate, VariableIDMap &variableIDs);
void translateDerivedPredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration, VariableIDMap &variableIDs);

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: avoid code duplication with translatePredicate
inline void translateDerivedPredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate, VariableIDMap &variableIDs)
{
	const auto &arguments = derivedPredicate.arguments;

	if (arguments.empty())
	{
		outputStream << *derivedPredicate.declaration;
		return;
	}

	outputStream << "(" << *derivedPredicate.declaration;

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

inline void translateDerivedPredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration, VariableIDMap &variableIDs)
{
	outputStream << colorlog::Keyword("derivedVariable") << "(";

	if (derivedPredicateDeclaration.parameters.empty())
	{
		outputStream << derivedPredicateDeclaration << ")";
		return;
	}

	outputStream << "(" << derivedPredicateDeclaration;
	translateVariablesForRuleHead(outputStream, derivedPredicateDeclaration.parameters, variableIDs);
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicateToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate, VariableIDMap &variableIDs, bool isPositive = true)
{
	outputStream << colorlog::Keyword("derivedVariable") << "(";

	translateDerivedPredicate(outputStream, derivedPredicate, variableIDs);

	outputStream
		<< "), "
		<< colorlog::Keyword("value") << "("
		<< colorlog::Keyword("derivedVariable") << "(";

	translateDerivedPredicate(outputStream, derivedPredicate, variableIDs);

	outputStream << "), ";

	if (isPositive)
		outputStream << colorlog::Boolean("true");
	else
		outputStream << colorlog::Boolean("false");

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicateDeclarationToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration, VariableIDMap &variableIDs, bool isPositive = true)
{
	translateDerivedPredicateDeclaration(outputStream, derivedPredicateDeclaration, variableIDs);
	outputStream << ", " << colorlog::Keyword("value") << "(";
	translateDerivedPredicateDeclaration(outputStream, derivedPredicateDeclaration, variableIDs);
	outputStream << ", ";

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
