#ifndef __PLASP__PDDL__EXPRESSION__N_ARY_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__N_ARY_EXPRESSION_H

#include <memory>
#include <string>
#include <vector>

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/VariableExpression.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NAryExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class NAryExpression: public Expression
{
	public:
		const Expressions &arguments() const;

	protected:
		template<typename ExpressionParser>
		void parse(utils::Parser &parser, Context &context, const VariableExpressions &parameters,
			ExpressionParser parseExpression);

	private:
		Expressions m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
void NAryExpression::parse(utils::Parser &parser, Context &context,
	const VariableExpressions &parameters, ExpressionParser parseExpression)
{
	parser.skipWhiteSpace();

	// Assume that expression identifier (and, or, etc.) is already parsed
	// Parse arguments of the expression
	while (parser.currentCharacter() != ')')
	{
		m_arguments.emplace_back(parseExpression(parser, context, parameters));

		parser.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
