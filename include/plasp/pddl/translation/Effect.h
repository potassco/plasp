#ifndef __PLASP__PDDL__TRANSLATION__EFFECT_H
#define __PLASP__PDDL__TRANSLATION__EFFECT_H

#include <colorlog/Formatting.h>

#include <pddlparse/NormalizedAST.h>

#include <plasp/TranslatorException.h>

#include <plasp/pddl/translation/Predicate.h>
#include <plasp/pddl/translation/Primitives.h>

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
inline void translateEffect(colorlog::ColorStream &outputStream, const ::pddl::normalizedAST::Effect &effect, const std::string &objectType, PrintObjectName printObjectName)
{
	const auto handleUnsupported =
		[](const auto &)
		{
			throw TranslatorException("only “and” expressions and (negated) predicates supported as action effects currently");
		};

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
			outputStream << ").";
		};

	const auto handleAtomicFormula =
		[&](const ::pddl::normalizedAST::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleUnsupported);
		};

	const auto handleNot =
		[&](const ::pddl::normalizedAST::NotPointer<::pddl::normalizedAST::Effect> &not_)
		{
			if (!not_->argument.is<::pddl::normalizedAST::AtomicFormula>() || !not_->argument.get<::pddl::normalizedAST::AtomicFormula>().is<::pddl::normalizedAST::PredicatePointer>())
				handleUnsupported(not_);

			const auto &predicate = not_->argument.get<::pddl::normalizedAST::AtomicFormula>().get<::pddl::normalizedAST::PredicatePointer>();

			handlePredicate(predicate, false);
		};

	const auto handleAnd =
		[&](const ::pddl::normalizedAST::AndPointer<::pddl::normalizedAST::Effect> &and_)
		{
			for (const auto &argument : and_->arguments)
				translateEffect(outputStream, argument, objectType, printObjectName);
		};

	effect.match(handleAtomicFormula, handleNot, handleAnd, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
