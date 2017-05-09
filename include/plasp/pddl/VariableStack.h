#ifndef __PLASP__PDDL__VARIABLE_STACK_H
#define __PLASP__PDDL__VARIABLE_STACK_H

#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableStack
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class VariableStack
{
	public:
		void push(expressions::Variables *variables);
		void pop();

		expressions::VariablePointer parseAndFind(Context &context);

	private:
		std::vector<expressions::Variables *> m_variableStack;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif