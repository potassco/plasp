#ifndef __PLASP__PDDL__EXPRESSIONS__BINARY_H
#define __PLASP__PDDL__EXPRESSIONS__BINARY_H

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
// Binary
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Binary: public Expression
{
	public:
		const Expression *leftArgument() const;
		const Expression *rightArgument() const;

	protected:
		template<typename ExpressionParser>
		void parse(Context &context, const Variables &parameters, ExpressionParser parseExpression);

	private:
		ExpressionPointer m_leftArgument;
		ExpressionPointer m_rightArgument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ExpressionParser>
void Binary::parse(Context &context, const Variables &parameters, ExpressionParser parseExpression)
{
	// Assume that expression identifier (imply, exists, etc.) is already parsed
	// Parse arguments of the expression
	m_leftArgument = parseExpression(context, parameters);
	m_rightArgument = parseExpression(context, parameters);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
