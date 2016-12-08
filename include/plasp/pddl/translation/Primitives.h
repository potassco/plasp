#ifndef __PLASP__PDDL__TRANSLATION__PRIMITIVES_H
#define __PLASP__PDDL__TRANSLATION__PRIMITIVES_H

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/translation/Predicate.h>

namespace plasp
{
namespace pddl
{
namespace translation
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Primitives
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void translateLiteral(output::ColorStream &outputStream, const Expression &literal);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateLiteral(output::ColorStream &outputStream, const Expression &literal)
{
	// Translate single predicate
	if (literal.is<expressions::Predicate>())
	{
		const auto &predicate = literal.as<expressions::Predicate>();

		outputStream << output::Keyword("variable") << "(";
		translation::translatePredicate(outputStream, predicate);
		outputStream << "), " << output::Keyword("value") << "(";
		translation::translatePredicate(outputStream, predicate);
		outputStream << ", " << output::Boolean("true") << ")";
	}
	// Assuming that "not" expression may only contain a predicate
	else if (literal.is<expressions::Not>())
	{
		const auto &notExpression = literal.as<expressions::Not>();

		if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
			throw output::TranslatorException("only negations of primitive predicates supported as literals currently");

		const auto &predicate = notExpression.argument()->as<expressions::Predicate>();

		outputStream << output::Keyword("variable") << "(";
		translation::translatePredicate(outputStream, predicate);
		outputStream << "), " << output::Keyword("value") << "(";
		translation::translatePredicate(outputStream, predicate);
		outputStream << ", " << output::Boolean("false") << ")";
	}
	else if (literal.is<expressions::DerivedPredicate>())
	{
		const auto &derivedPredicate = literal.as<expressions::DerivedPredicate>();

		outputStream << output::Keyword("derivedVariable") << "(";
		translation::translateDerivedPredicate(outputStream, derivedPredicate);
		outputStream << "), " << output::Keyword("value") << "(";
		translation::translateDerivedPredicate(outputStream, derivedPredicate);
		outputStream << ", " << output::Boolean("true") << ")";
	}
	else
		throw output::TranslatorException("only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
