#ifndef __PLASP__PDDL__EXPRESSIONS__N_ARY_H
#define __PLASP__PDDL__EXPRESSIONS__N_ARY_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NAry
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class NAry: public ExpressionCRTP<Derived>
{
	public:
		template<typename ExpressionParser>
		static std::unique_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		const Expressions &arguments() const;

	private:
		Expressions m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
std::unique_ptr<Derived> NAry<Derived>::parse(Context &context,
	ExpressionContext &expressionContext, ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("(")
		|| !parser.probeIdentifier(Derived::Identifier, isIdentifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = std::make_unique<Derived>();

	parser.skipWhiteSpace();

	// Assume that expression identifier (and, or, etc.) is already parsed
	// Parse arguments of the expression
	while (parser.currentCharacter() != ')')
	{
		expression->m_arguments.emplace_back(parseExpression(context, expressionContext));

		parser.skipWhiteSpace();
	}

	if (expression->m_arguments.empty())
		context.logger.parserWarning(context.parser, "\"" + Derived::Identifier + "\" expressions should not be empty");

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const Expressions &NAry<Derived>::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
