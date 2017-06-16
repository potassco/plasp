#ifndef __PDDL_PARSE__DETAIL__VARIABLE_STACK_H
#define __PDDL_PARSE__DETAIL__VARIABLE_STACK_H

#include <pddlparse/ASTForward.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableStack
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class VariableStack
{
	public:
		using Layer = ast::VariableDeclarations *;

	public:
		void push(Layer layer);
		void pop();

		std::experimental::optional<ast::VariableDeclaration *> findVariableDeclaration(const std::string &variableName);

	private:
		std::vector<Layer> m_layers;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
