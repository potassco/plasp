#ifndef __PLASP__PDDL__TRANSLATION__GOAL_H
#define __PLASP__PDDL__TRANSLATION__GOAL_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>

#include <plasp/TranslatorException.h>

#include <plasp/pddl/translation/DerivedPredicate.h>
#include <plasp/pddl/translation/Predicate.h>
#include <plasp/pddl/translation/Primitives.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Goal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateGoal(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Goal &goal)
{
	VariableIDMap variableIDs;

	const auto ensureNoVariables =
		[](const auto &predicate)
		{
			for (const auto &argument : predicate->arguments)
				if (argument.template is<::pddl::normalizedAST::VariablePointer>())
					throw TranslatorException("goal descriptions must be variable-free");
		};

	const auto handlePredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate, bool isPositive = true)
		{
			ensureNoVariables(predicate);

			outputStream << std::endl << colorlog::Function("goal") << "(";
			translatePredicateToVariable(outputStream, *predicate, variableIDs, isPositive);
			outputStream << ").";
		};

	const auto handleNegatedPredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate)
		{
			handlePredicate(predicate, false);
		};

	const auto handleDerivedPredicate =
		[&](const ::pddl::normalizedAST::DerivedPredicatePointer &derivedPredicate, bool isPositive = true)
		{
			ensureNoVariables(derivedPredicate);

			outputStream << std::endl << colorlog::Function("goal") << "(";
			translateDerivedPredicateToVariable(outputStream, *derivedPredicate, variableIDs, isPositive);
			outputStream << ").";
		};

	const auto handleNegatedDerivedPredicate =
		[&](const ::pddl::normalizedAST::DerivedPredicatePointer &derivedPredicate)
		{
			handleDerivedPredicate(derivedPredicate, false);
		};

	const auto handleAtomicFormula =
		[&](const ::pddl::normalizedAST::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleDerivedPredicate);
		};

	const auto handleNot =
		[&](const ::pddl::normalizedAST::NotPointer<::pddl::normalizedAST::AtomicFormula> &not_)
		{
			not_->argument.match(handleNegatedPredicate, handleNegatedDerivedPredicate);
		};

	const auto handleLiteral =
		[&](const ::pddl::normalizedAST::Literal &literal)
		{
			literal.match(handleAtomicFormula, handleNot);
		};

	const auto handleAnd =
		[&](const ::pddl::normalizedAST::AndPointer<::pddl::normalizedAST::Literal> &and_)
		{
			for (const auto &argument : and_->arguments)
				handleLiteral(argument);
		};

	goal.match(handleLiteral, handleAnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
