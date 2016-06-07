#ifndef __PLASP__PDDL__EXPRESSIONS__VARIABLE_H
#define __PLASP__PDDL__EXPRESSIONS__VARIABLE_H

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
		static void parseTypedDeclaration(Context &context, ExpressionContext &expressionContext);

		static const Variable *parseAndFind(Context &context, const ExpressionContext &expressionContext);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const Expression *type() const;

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(const Expression *type);

	private:
		static void parseDeclaration(Context &context, Variables &parameters);

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
