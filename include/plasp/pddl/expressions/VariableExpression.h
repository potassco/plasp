#ifndef __PLASP__PDDL__EXPRESSION__VARIABLE_EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION__VARIABLE_EXPRESSION_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableExpression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class VariableExpression: public Expression
{
	public:
		static VariableExpressionPointer parseDeclaration(utils::Parser &parser);
		static void parseTypedDeclaration(utils::Parser &parser, Context &context,
			VariableExpressions &variableExpressions);

		static const VariableExpression *parse(utils::Parser &parser,
			const VariableExpressions &variableExpressions);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		TypePtr type() const;

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(TypePtr type);

	private:
		VariableExpression();

		bool m_isDirty;

		std::string m_name;

		TypePtr m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
