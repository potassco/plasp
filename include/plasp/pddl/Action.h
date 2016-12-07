#ifndef __PLASP__PDDL__ACTION_H
#define __PLASP__PDDL__ACTION_H

#include <vector>

#include <plasp/input/Parser.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Action
{
	public:
		static void parseDeclaration(Context &context, Domain &domain);

	public:
		const std::string &name() const;

		const expressions::Variables &parameters() const;
		const Expression *precondition() const;
		const Expression *effect() const;

		void normalize(expressions::DerivedPredicates &derivedPredicates);

	private:
		Action() = default;

		std::string m_name;

		expressions::Variables m_parameters;
		ExpressionPointer m_precondition;
		ExpressionPointer m_effect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
