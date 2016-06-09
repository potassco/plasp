#ifndef __PLASP__PDDL__EXPRESSIONS__PREDICATE_H
#define __PLASP__PDDL__EXPRESSIONS__PREDICATE_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Predicate: public Expression
{
	public:
		static PredicatePointer parse(Context &context, ExpressionContext &expressionContext);
		static PredicatePointer parse(Context &context, const Problem &problem);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const Expressions &arguments() const;

		bool isDeclared() const;

	private:
		Predicate();

		void setDeclared();

		bool m_isDeclared;

		std::string m_name;
		Expressions m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
