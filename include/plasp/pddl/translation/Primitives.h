#ifndef __PLASP__PDDL__TRANSLATION__PRIMITIVES_H
#define __PLASP__PDDL__TRANSLATION__PRIMITIVES_H

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>

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

void translatePredicate(output::ColorStream &outputStream, const expressions::Predicate &predicate);
void translateLiteral(output::ColorStream &outputStream, const Expression &literal);

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translatePredicate(output::ColorStream &outputStream, const expressions::Predicate &predicate)
{
	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		outputStream << output::String(predicate.name().c_str());

		return;
	}

	outputStream << "(" << output::String(predicate.name().c_str());

	for (const auto &argument : arguments)
	{
		outputStream << ", ";

		if (argument->is<expressions::Constant>())
		{
			const auto &constant = argument->as<expressions::Constant>();

			outputStream << output::Keyword("constant") << "(" << output::String(constant.name().c_str()) << ")";
		}
		else if (argument->is<expressions::Variable>())
		{
			const auto &variable = argument->as<expressions::Variable>();

			outputStream << output::Variable(variable.name().c_str());
		}
		else
			throw output::TranslatorException("only variables and constants supported in predicates currently");
	}

	outputStream << ")";
}

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
		//const auto &derivedPredicate = literal.as<expressions::DerivedPredicate>();

		/*m_outputStream << output::Keyword("variable") << "(";
		translation::translatePredicate(predicate);
		m_outputStream << "), " << output::Keyword("value") << "(";
		translation::translatePredicate(predicate);
		m_outputStream << ", " << output::Boolean("true") << ")";*/

		outputStream << "(derived predicate)";
	}
	else
		throw output::TranslatorException("only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
