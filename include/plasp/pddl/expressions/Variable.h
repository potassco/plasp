#ifndef __PLASP__PDDL__EXPRESSION__VARIABLE_H
#define __PLASP__PDDL__EXPRESSION__VARIABLE_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Type.h>

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
			Variables &variables);

		static const Variable *parse(utils::Parser &parser, const Variables &variables);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		TypePtr type() const;

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(TypePtr type);

	private:
		Variable();

		bool m_isDirty;

		std::string m_name;

		TypePtr m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
