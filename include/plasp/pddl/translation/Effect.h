#ifndef __PLASP__PDDL__TRANSLATION__EFFECT_H
#define __PLASP__PDDL__TRANSLATION__EFFECT_H

#include <colorlog/Formatting.h>

#include <pddlparse/AST.h>

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
inline void translateEffect(colorlog::ColorStream &outputStream, const ::pddl::ast::Effect &effect, const std::string &objectType, PrintObjectName printObjectName)
{
	const auto handleUnsupported =
		[](const auto &)
		{
			throw TranslatorException("only “and” expressions and (negated) predicates supported as action effects currently");
		};

	const auto handlePredicate =
		[&](const ::pddl::ast::PredicatePointer &predicate, bool isPositive = true)
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
		[&](const ::pddl::ast::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleUnsupported);
		};

	const auto handleNot =
		[&](const ::pddl::ast::NotPointer<::pddl::ast::Effect> &not_)
		{
			if (!not_->argument.is<::pddl::ast::AtomicFormula>() || !not_->argument.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
				handleUnsupported(not_);

			const auto &predicate = not_->argument.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			handlePredicate(predicate, false);
		};

	const auto handleAnd =
		[&](const ::pddl::ast::AndPointer<::pddl::ast::Effect> &and_)
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
