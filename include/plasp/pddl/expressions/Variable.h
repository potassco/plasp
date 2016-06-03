#ifndef __PLASP__PDDL__EXPRESSION__VARIABLE_H
#define __PLASP__PDDL__EXPRESSION__VARIABLE_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Variable: public Expression
{
	public:
		static VariablePointer parseDeclaration(utils::Parser &parser);

		static void parseTypedDeclaration(utils::Parser &parser, Context &context,
			Variables &parameters);

		static const Variable *parseExisting(utils::Parser &parser, const Variables &variables);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const Expression *type() const;

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(const Expression *type);

	private:
		Variable();

		bool m_isDirty;

		std::string m_name;

		const Expression *m_type;

		// Stores "either" expression if necessary
		ExpressionPointer m_eitherExpression;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
