#ifndef __PLASP__PDDL__TRANSLATION__CONDITIONAL_EFFECT_H
#define __PLASP__PDDL__TRANSLATION__CONDITIONAL_EFFECT_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>

#include <plasp/TranslatorException.h>

#include <plasp/pddl/translation/Predicate.h>
#include <plasp/pddl/translation/Primitives.h>
#include <plasp/pddl/translation/Variables.h>
#include <plasp/pddl/translation/VariableStack.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ConditionalEffect
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translateConditionalEffect(colorlog::ColorStream &outputStream,
	const ::pddl::normalizedAST::ConditionalEffect &conditionalEffect,
	PrintObjectName printObjectName, VariableStack &variableStack,
	size_t &numberOfConditionalEffects, VariableIDMap &variableIDs)
{
	const auto handlePredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("postcondition") << "(";
			printObjectName();
			outputStream
				<< ", " << colorlog::Keyword("effect") << "("
				<< colorlog::Number<size_t>(numberOfConditionalEffects) << ")"
				<< ", ";
			translatePredicateToVariable(outputStream, *predicate, variableIDs, isPositive);
			outputStream << ") :- " << colorlog::Function("action") << "(";
			printObjectName();
			outputStream << ")";

			if (!variableStack.layers.empty())
				for (const auto &layer : variableStack.layers)
				{
					if (!layer->empty())
						outputStream << ", ";

					translateVariablesForRuleBody(outputStream, *layer, variableIDs);
				}

			outputStream << ".";
		};

	const auto handleNegatedPredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate)
		{
			handlePredicate(predicate, false);
		};

	const auto handleDerivedPredicate =
		[&](const ::pddl::normalizedAST::DerivedPredicatePointer &, bool = true)
		{
			// TODO: refactor as to exclude derived predicates from the Effect variant
			throw TranslatorException("derived predicates should not appear in effects, please report to the bug tracker");
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

	conditionalEffect.match(handleAnd, handleLiteral);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
