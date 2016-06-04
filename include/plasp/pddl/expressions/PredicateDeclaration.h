#ifndef __PLASP__PDDL__EXPRESSIONS__PREDICATE_DECLARATION_H
#define __PLASP__PDDL__EXPRESSIONS__PREDICATE_DECLARATION_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PredicateDeclaration: public Expression
{
	public:
		static void parse(Context &context);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const Variables &arguments() const;

		bool isDeclared() const;

	private:
		PredicateDeclaration();

		void setDeclared();

		bool m_isDeclared;

		std::string m_name;
		Variables m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
