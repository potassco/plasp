#ifndef __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_PRECONDITION_H
#define __PLASP__PDDL__TRANSLATION__DERIVED_PREDICATE_PRECONDITION_H

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
// DerivedPredicatePrecondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translateDerivedPredicatePrecondition(colorlog::ColorStream &outputStream,
	const ::pddl::normalizedAST::DerivedPredicatePrecondition &derivedPredicatePrecondition,
	const std::string &objectType, PrintObjectName printObjectName, VariableIDMap &variableIDs)
{
	const auto handlePredicate =
		[&](const ::pddl::normalizedAST::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
			translatePredicateToVariable(outputStream, *predicate, variableIDs, isPositive);
			outputStream << ") :- " << colorlog::Function(objectType.c_str()) << "(";
			printObjectName();
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
			outputStream << std::endl << colorlog::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
			translateDerivedPredicateToVariable(outputStream, *derivedPredicate, variableIDs, isPositive);
			outputStream << ") :- " << colorlog::Function(objectType.c_str()) << "(";
			printObjectName();
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

	const auto handleOr =
		[&](const ::pddl::normalizedAST::OrPointer<::pddl::normalizedAST::Literal> &or_)
		{
			for (const auto &argument : or_->arguments)
				handleLiteral(argument);
		};

	derivedPredicatePrecondition.match(handleLiteral, handleAnd, handleOr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
