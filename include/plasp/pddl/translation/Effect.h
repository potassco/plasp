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

	const auto handleDerivedPredicate =
		[&](const ::pddl::normalizedAST::DerivedPredicatePointer &, bool = true)
		{
			throw TranslatorException("derived predicates not yet supported by translator");
		};

	const auto handleAtomicFormula =
		[&](const ::pddl::normalizedAST::AtomicFormula &atomicFormula)
		{
			atomicFormula.match(handlePredicate, handleDerivedPredicate);
		};

	const auto handleNot =
		[&](const ::pddl::normalizedAST::NotPointer<::pddl::normalizedAST::AtomicFormula> &not_)
		{
			if (!not_->argument.is<::pddl::normalizedAST::PredicatePointer>())
				throw TranslatorException("only “and” expressions and (negated) predicates supported as action effects currently");

			const auto &predicate = not_->argument.get<::pddl::normalizedAST::PredicatePointer>();

			handlePredicate(predicate, false);
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
				translateEffect(outputStream, argument, objectType, printObjectName);
		};

	const auto handleForAll =
		[&](const ::pddl::normalizedAST::ForAllPointer<::pddl::normalizedAST::Effect> &)
		{
			throw TranslatorException("“when” expressions not yet supported by translator");
		};

	const auto handleWhen =
		[&](const ::pddl::normalizedAST::WhenPointer<::pddl::normalizedAST::Precondition, ::pddl::normalizedAST::ConditionalEffect> &)
		{
			throw TranslatorException("“when” expressions not yet supported by translator");
		};

	effect.match(handleAnd, handleForAll, handleLiteral, handleWhen);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
