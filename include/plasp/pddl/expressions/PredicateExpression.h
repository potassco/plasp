#ifndef __PLASP__PDDL__EXPRESSION__PREDICATE_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__PREDICATE_EXPRESSION_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/Predicate.h>

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
		static std::unique_ptr<PredicateExpression> parse(std::string name, utils::Parser &parser, Context &context, const Variables &parameters);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

	private:
		PredicateExpression() = default;

		std::string m_name;
		Variables m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
