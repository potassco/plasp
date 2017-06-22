#include <plasp/sas/Variable.h>

#include <iostream>

#include <tokenize/TokenizerException.h>

#include <colorlog/Formatting.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Variable::Variable()
:	m_axiomLayer(-1)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Variable Variable::fromSAS(tokenize::Tokenizer<> &tokenizer)
{
	Variable variable;

	tokenizer.expect<std::string>("begin_variable");
	tokenizer.expect<std::string>("var");

	variable.m_name = tokenizer.get<std::string>();
	variable.m_axiomLayer = tokenizer.get<int>();

	const auto numberOfValues = tokenizer.get<size_t>();
	variable.m_values.reserve(numberOfValues);

	for (size_t j = 0; j < numberOfValues; j++)
	{
		variable.m_values.emplace_back(Value::fromSAS(tokenizer));

		// <none of those> values are only allowed at the end
		if (j < numberOfValues - 1 && variable.m_values[j] == Value::None)
			throw tokenize::TokenizerException(tokenizer.location(), "<none of those> value must be the last value of a variable");
	}

	tokenizer.expect<std::string>("end_variable");

	return variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Variable::printNameAsASPPredicate(colorlog::ColorStream &stream) const
{
	// TODO: assert that name is a number indeed
	stream << colorlog::Keyword("variable") << "(" << colorlog::Number<std::string>(m_name) << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variable &Variable::referenceFromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables)
{
	const auto variableID = tokenizer.get<size_t>();

	if (variableID >= variables.size())
		throw tokenize::TokenizerException(tokenizer.location(), "variable index out of range (index " + std::to_string(variableID) + ")");

	return variables[variableID];
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Variable::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Variable::axiomLayer() const
{
	return m_axiomLayer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Values &Variable::values() const
{
	return m_values;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
