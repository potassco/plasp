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
	auto &tokenizer = context.tokenizer;

	tokenizer.skipWhiteSpace();

	tokenizer.expect<std::string>("?");

	const auto variableName = tokenizer.getIdentifier();

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

	throw tokenize::TokenizerException(tokenizer, "variable “" + variableName + "” used but never declared");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
