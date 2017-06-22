#ifndef __PLASP__PDDL__TRANSLATION__GOAL_H
#define __PLASP__PDDL__TRANSLATION__GOAL_H

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
// Goal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateGoal(colorlog::ColorStream &outputStream, const ::pddl::ast::Goal &goal)
{
	const auto handleUnsupported =
		[](const auto &)
		{
			throw TranslatorException("only “and” expressions and (negated) predicates supported as goals currently");
		};

	const auto handlePredicate =
		[&](const ::pddl::ast::PredicatePointer &predicate, bool isPositive = true)
		{
			outputStream << std::endl << colorlog::Function("goal") << "(";
			// TODO: assert that goal is variable-free
			translatePredicateToVariable(outputStream, *predicate, isPositive);
			outputStream << ").";
		};

	const auto handleAtomicFormula =
		[&](const ::pddl::ast::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleUnsupported);
		};

	const auto handleNot =
		[&](const ::pddl::ast::NotPointer<::pddl::ast::Goal> &not_)
		{
			if (!not_->argument.is<::pddl::ast::AtomicFormula>() || !not_->argument.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
				handleUnsupported(not_);

			const auto &predicate = not_->argument.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			handlePredicate(predicate, false);
		};

	const auto handleAnd =
		[&](const ::pddl::ast::AndPointer<::pddl::ast::Goal> &and_)
		{
			for (const auto &argument : and_->arguments)
				translateGoal(outputStream, argument);
		};

	goal.match(handleAtomicFormula, handleNot, handleAnd, handleUnsupported);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
