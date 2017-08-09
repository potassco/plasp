#ifndef __PDDL__NORMALIZED_AST_OUTPUT_H
#define __PDDL__NORMALIZED_AST_OUTPUT_H

#include <colorlog/ColorStream.h>
#include <colorlog/Formatting.h>

#include <pddl/ASTOutput.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/OutputUtils.h>
#include <pddl/detail/parsing/Requirement.h>

namespace pddl
{
namespace normalizedAST
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Output
//
////////////////////////////////////////////////////////////////////////////////////////////////////

using ast::print;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const DerivedPredicate &derivedPredicate, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const DerivedPredicateDeclaration &derivedPredicateDeclaration, pddl::detail::PrintContext &printContext);

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Requirement &requirement, pddl::detail::PrintContext &printContext);
inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, pddl::detail::PrintContext &printContext);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Compounds
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const DerivedPredicate &derivedPredicate, pddl::detail::PrintContext &printContext)
{
	// TODO: implement correctly

	stream << "(" << pddl::detail::Identifier(derivedPredicate.declaration->name);

	for (const auto &argument : derivedPredicate.arguments)
	{
		stream << " ";
		print(stream, argument, printContext);
	}

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const DerivedPredicateDeclaration &derivedPredicateDeclaration, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(":derived-predicate") << " " << pddl::detail::Identifier(derivedPredicateDeclaration.name);

	printContext.indentationLevel++;

	if (!derivedPredicateDeclaration.parameters.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":parameters");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(";

		for (const auto &parameter : derivedPredicateDeclaration.parameters)
		{
			if (&parameter != &derivedPredicateDeclaration.parameters.front())
				pddl::detail::printIndentedNewline(stream, printContext);

			print(stream, *parameter, printContext);
		}

		stream << ")";

		printContext.indentationLevel--;
	}

	if (!derivedPredicateDeclaration.existentialParameters.empty())
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":exists");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		stream << "(";

		for (const auto &parameter : derivedPredicateDeclaration.existentialParameters)
		{
			if (parameter.get() != derivedPredicateDeclaration.existentialParameters.front().get())
				pddl::detail::printIndentedNewline(stream, printContext);

			print(stream, *parameter, printContext);
		}

		stream << ")";

		printContext.indentationLevel--;
	}

	if (derivedPredicateDeclaration.precondition)
	{
		pddl::detail::printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":precondition");

		printContext.indentationLevel++;

		pddl::detail::printIndentedNewline(stream, printContext);
		print(stream, derivedPredicateDeclaration.precondition.value(), printContext);

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PDDL Structure
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Action &action, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword(":action") << " " << pddl::detail::Identifier(action.name);

	printContext.indentationLevel++;

	if (!action.parameters.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":parameters");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		stream << "(";
		print(stream, action.parameters, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (action.precondition)
	{
		printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":precondition");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, action.precondition.value(), printContext);

		printContext.indentationLevel--;
	}

	if (action.effect)
	{
		printIndentedNewline(stream, printContext);
		stream << colorlog::Keyword(":effect");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, action.effect.value(), printContext);

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Domain &domain, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("domain") << " " << pddl::detail::Identifier(domain.name) << ")";

	printContext.indentationLevel++;

	if (!domain.types.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":types");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.types, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.constants.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":constants");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.constants, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.predicates.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":predicates");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, domain.predicates, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!domain.derivedPredicates.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, domain.derivedPredicates, printContext);
	}

	if (!domain.actions.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, domain.actions, printContext);
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const InitialState &initialState, pddl::detail::PrintContext &printContext)
{
	assert(!initialState.facts.empty());

	stream << "(" << colorlog::Keyword(":init");

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	print(stream, initialState.facts, printContext);

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Problem &problem, pddl::detail::PrintContext &printContext)
{
	stream << "(" << colorlog::Keyword("define") << " (" << colorlog::Keyword("problem") << " " << pddl::detail::Identifier(problem.name) << ")";

	printContext.indentationLevel++;

	printIndentedNewline(stream, printContext);
	stream << "(" << colorlog::Keyword(":domain") << " " << pddl::detail::Identifier(problem.domain->name) << ")";

	if (!problem.derivedPredicates.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, problem.derivedPredicates, printContext);
	}

	if (!problem.objects.empty())
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":objects");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, problem.objects, printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	if (!problem.initialState.facts.empty())
	{
		printIndentedNewline(stream, printContext);
		print(stream, problem.initialState, printContext);
	}

	if (problem.goal)
	{
		printIndentedNewline(stream, printContext);
		stream << "(" << colorlog::Keyword(":goal");

		printContext.indentationLevel++;

		printIndentedNewline(stream, printContext);
		print(stream, problem.goal.value(), printContext);
		stream << ")";

		printContext.indentationLevel--;
	}

	printContext.indentationLevel--;

	return (stream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &print(colorlog::ColorStream &stream, const Description &description, pddl::detail::PrintContext &printContext)
{
	print(stream, *description.domain, printContext);
	stream << std::endl;

	if (description.problem)
	{
		stream << std::endl;
		print(stream, *description.problem.value(), printContext);
		stream << std::endl;
	}

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Description &description)
{
	pddl::detail::PrintContext printContext;

	return print(stream, description, printContext);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
