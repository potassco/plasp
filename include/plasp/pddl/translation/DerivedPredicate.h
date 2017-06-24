#ifndef __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_H
#define __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_H

#include <colorlog/Formatting.h>

#include <pddlparse/NormalizedAST.h>
#include <pddlparse/Parse.h>

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

void translateDerivedPredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate);
void translateDerivedPredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicate(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate)
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
				outputStream << *variable;
			};

		argument.match(handleConstant, handleVariable);
	}

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicateDeclaration(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration)
{
	outputStream << colorlog::Keyword("derivedVariable") << "(";

	if (derivedPredicateDeclaration.parameters.empty())
	{
		outputStream << derivedPredicateDeclaration << ")";
		return;
	}

	outputStream << "(" << derivedPredicateDeclaration;
	translateVariablesForRuleHead(outputStream, derivedPredicateDeclaration.parameters);
	outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicateToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate, bool isPositive = true)
{
	outputStream << colorlog::Keyword("derivedVariable") << "(";
	translateDerivedPredicate(outputStream, derivedPredicate);
	outputStream << "), " << colorlog::Keyword("value") << "(";
	translateDerivedPredicate(outputStream, derivedPredicate);
	outputStream << ", ";

	if (isPositive)
		outputStream << colorlog::Boolean("true");
	else
		outputStream << colorlog::Boolean("false");

	outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateDerivedPredicateDeclarationToVariable(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration, bool isPositive = true)
{
	outputStream << colorlog::Keyword("derivedVariable") << "(";
	translateDerivedPredicateDeclaration(outputStream, derivedPredicateDeclaration);
	outputStream << "), " << colorlog::Keyword("value") << "(";
	translateDerivedPredicateDeclaration(outputStream, derivedPredicateDeclaration);
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
