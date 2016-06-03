#ifndef __PLASP__PDDL__EXPRESSION__PREDICATE_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__PREDICATE_EXPRESSION_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Predicate.h>
#include <plasp/pddl/expressions/VariableExpression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PredicateExpression: public Expression
{
	public:
		static PredicateExpressionPointer parse(std::string name, utils::Parser &parser,
			Context &context, const VariableExpressions &parameters);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::vector<const VariableExpression *> &arguments() const;

	private:
		PredicateExpression() = default;

		std::string m_name;
		std::vector<const VariableExpression *> m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
