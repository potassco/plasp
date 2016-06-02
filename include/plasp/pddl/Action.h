#ifndef __PLASP__PDDL__ACTION_H
#define __PLASP__PDDL__ACTION_H

#include <vector>

#include <plasp/pddl/Variable.h>
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

	private:
		Action(std::string name);

		void parsePrecondition(utils::Parser &parser, Context &context);

		std::string m_name;

		std::vector<Variable> m_parameters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
