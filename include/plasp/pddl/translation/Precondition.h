#ifndef __PLASP__PDDL__TRANSLATION__PRECONDITION_H
#define __PLASP__PDDL__TRANSLATION__PRECONDITION_H

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
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName, typename PrintRuleBody>
inline void translatePrecondition(colorlog::ColorStream &outputStream,
	const ::pddl::normalizedAST::Precondition &precondition, PrintObjectName printObjectName,
	PrintRuleBody printRuleBody, VariableIDMap &variableIDs)
{
	const auto handlePredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
			translatePredicateToVariable(outputStream, *predicate, variableIDs, isPositive);
			outputStream << ")";
			printRuleBody();
			outputStream << ".";
		};

	const auto handleNegatedPredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate)
		{
			handlePredicate(predicate, false);
		};

	const auto handleDerivedPredicate =
		[&](const ::pddl::normalizedAST::DerivedPredicatePointer &derivedPredicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
			translateDerivedPredicateToVariable(outputStream, *derivedPredicate, variableIDs, isPositive);
			outputStream << ")";
			printRuleBody();
			outputStream << ".";
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

	precondition.match(handleLiteral, handleAnd);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
