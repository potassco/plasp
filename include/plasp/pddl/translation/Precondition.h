#ifndef __PLASP__PDDL__TRANSLATION__PRECONDITION_H
#define __PLASP__PDDL__TRANSLATION__PRECONDITION_H

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
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename PrintObjectName>
inline void translatePrecondition(colorlog::ColorStream &outputStream, const ::pddl::ast::Precondition &precondition, const std::string &objectType, PrintObjectName printObjectName)
{
	const auto handleUnsupported =
		[](const auto &)
		{
			throw TranslatorException("only “and” expressions and (negated) predicates supported as action preconditions currently");
		};

	const auto handlePredicate =
		[&](const ::pddl::ast::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("precondition") << "(";
			printObjectName();
			outputStream << ", ";
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
		[&](const ::pddl::ast::NotPointer<::pddl::ast::Precondition> &not_)
		{
			if (!not_->argument.is<::pddl::ast::AtomicFormula>() || !not_->argument.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
				handleUnsupported(not_);

			const auto &predicate = not_->argument.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			handlePredicate(predicate, false);
		};

	const auto handleAnd =
		[&](const ::pddl::ast::AndPointer<::pddl::ast::Precondition> &and_)
		{
			for (const auto &argument : and_->arguments)
				translatePrecondition(outputStream, argument, objectType, printObjectName);
		};

	precondition.match(handleAtomicFormula, handleNot, handleAnd, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
