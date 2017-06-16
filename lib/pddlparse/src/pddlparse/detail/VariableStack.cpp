#include <pddlparse/detail/VariableStack.h>

#include <algorithm>

#include <pddlparse/AST.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableStack
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableStack::push(ast::VariableDeclarations *layer)
{
	m_layers.push_back(layer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableStack::pop()
{
	m_layers.pop_back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::VariableDeclaration *> VariableStack::findVariableDeclaration(const std::string &variableName)
{
	const auto variableDeclarationMatches =
		[&variableName](const auto &variableDeclaration)
		{
			return variableDeclaration->name == variableName;
		};

	for (auto i = m_layers.rbegin(); i != m_layers.rend(); i++)
	{
		auto &layer = **i;
		const auto matchingVariableDeclaration = std::find_if(layer.begin(), layer.end(), variableDeclarationMatches);

		if (matchingVariableDeclaration != layer.end())
			return matchingVariableDeclaration->get();
	}

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
