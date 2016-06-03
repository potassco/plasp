#ifndef __PLASP__PDDL__ACTION_H
#define __PLASP__PDDL__ACTION_H

#include <vector>

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Variable.h>
#include <plasp/utils/Parser.h>

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
		static Action &parseDeclaration(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;

		const expressions::Variables &parameters() const;
		const Expression &precondition() const;
		const Expression &effect() const;

	private:
		Action(std::string name);

		std::string m_name;

		expressions::Variables m_parameters;
		std::unique_ptr<Expression> m_precondition;
		std::unique_ptr<Expression> m_effect;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif