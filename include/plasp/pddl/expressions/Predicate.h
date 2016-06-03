#ifndef __PLASP__PDDL__EXPRESSION__PREDICATE_H
#define __PLASP__PDDL__EXPRESSION__PREDICATE_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Predicate.h>
#include <plasp/pddl/expressions/Variable.h>

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
		static PredicatePointer parse(std::string name, utils::Parser &parser,
			Context &context, const Variables &parameters);

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::vector<const Variable *> &arguments() const;

	private:
		Predicate() = default;

		std::string m_name;
		std::vector<const Variable *> m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
