#ifndef __PLASP__PDDL__EXPRESSIONS__N_ARY_H
#define __PLASP__PDDL__EXPRESSIONS__N_ARY_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
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

class NAry: public Expression
{
	public:
		const Expressions &arguments() const;

	protected:
		template<typename ExpressionParser>
		void parse(Context &context, const Variables &parameters, ExpressionParser parseExpression);

	private:
		Expressions m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
void NAry::parse(Context &context, const Variables &parameters, ExpressionParser parseExpression)
{
	context.parser.skipWhiteSpace();

	// Assume that expression identifier (and, or, etc.) is already parsed
	// Parse arguments of the expression
	while (context.parser.currentCharacter() != ')')
	{
		m_arguments.emplace_back(parseExpression(context, parameters));

		context.parser.skipWhiteSpace();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
