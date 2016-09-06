#include <plasp/pddl/VariableStack.h>

#include <plasp/pddl/Context.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableStack
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableStack::push(expressions::Variables *variables)
{
	m_variableStack.push_back(variables);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void VariableStack::pop()
{
	m_variableStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

expressions::VariablePointer VariableStack::parseAndFind(plasp::pddl::Context &context)
{
	auto &parser = context.parser;

	parser.skipWhiteSpace();

	parser.expect<std::string>("?");

	const auto variableName = parser.parseIdentifier();

	for (auto i = m_variableStack.crbegin(); i != m_variableStack.crend(); i++)
	{
		const auto *variables = *i;

		BOOST_ASSERT(variables);

		const auto match = std::find_if(variables->crbegin(), variables->crend(),
			[&](const auto &variable)
			{
				return variable->name() == variableName;
			});

		if (match != variables->crend())
			return match->get();
	}

	throw utils::ParserException(parser.coordinate(), "variable “" + variableName + "” used but never declared");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
