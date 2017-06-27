#ifndef __PLASP__PDDL__TRANSLATION__EFFECT_H
#define __PLASP__PDDL__TRANSLATION__EFFECT_H

#include <colorlog/Formatting.h>

#include <pddlparse/NormalizedAST.h>

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
// Effect
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translateEffect(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Effect &effect, const std::string &objectType, PrintObjectName printObjectName, VariableStack &variableStack)
{
	const auto handlePredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("postcondition") << "(";
			printObjectName();
			outputStream
				<< ", " << colorlog::Keyword("effect") << "("
				<< colorlog::Reserved("unconditional") << ")"
				<< ", ";
			translatePredicateToVariable(outputStream, *predicate, isPositive);
			outputStream << ") :- " << colorlog::Function(objectType.c_str()) << "(";
			printObjectName();
			outputStream << ")";

			if (!variableStack.layers.empty())
				for (const auto &layer : variableStack.layers)
				{
					if (!layer->empty())
						outputStream << ", ";

					translateVariablesForRuleBody(outputStream, *layer);
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
		[&](const ::pddl::normalizedAST::AndPointer<::pddl::normalizedAST::Effect> &and_)
		{
			for (const auto &argument : and_->arguments)
				translateEffect(outputStream, argument, objectType, printObjectName, variableStack);
		};

	const auto handleForAll =
		[&](const ::pddl::normalizedAST::ForAllPointer<::pddl::normalizedAST::Effect> &forAll)
		{
			variableStack.push(&forAll->parameters);

			translateEffect(outputStream, forAll->argument, objectType, printObjectName, variableStack);

			variableStack.pop();
		};

	const auto handleWhen =
		[&](const ::pddl::normalizedAST::WhenPointer<::pddl::normalizedAST::Precondition, ::pddl::normalizedAST::ConditionalEffect> &)
		{
			throw TranslatorException("“when” expressions not yet supported by translator");
		};

	effect.match(handleAnd, handleForAll, handleLiteral, handleWhen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translateEffect(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Effect &effect, const std::string &objectType, PrintObjectName printObjectName)
{
	VariableStack variableStack;

	translateEffect(outputStream, effect, objectType, printObjectName, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
