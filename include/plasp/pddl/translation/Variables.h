#ifndef __PLASP__PDDL__TRANSLATION__VARIABLES_H
#define __PLASP__PDDL__TRANSLATION__VARIABLES_H

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>

namespace plasp
{
namespace pddl
{
namespace translation
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
void translateVariablesForRuleHead(output::ColorStream &outputStream, const T &variables);
template<class T>
void translateVariablesForRuleBody(output::ColorStream &outputStream, const T &variables);

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline void translateVariablesForRuleHead(output::ColorStream &outputStream, const T &variables)
{
	if (variables.empty())
		return;

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		const auto &variable = **i;

		outputStream << ", " << output::Variable(variable.name().c_str());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline void translateVariablesForRuleBody(output::ColorStream &outputStream, const T &variables)
{
	if (variables.empty())
		return;

	outputStream << " :- ";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		const auto &variable = **i;

		if (i != variables.cbegin())
			outputStream << ", ";

		if (variable.type() != nullptr)
		{
			if (variable.type()->expressionType() != Expression::Type::PrimitiveType)
				throw output::TranslatorException("only primitive types supported currently");

			const auto &type = variable.type()->template as<expressions::PrimitiveType>();

			outputStream << output::Function("has") << "("
				<< output::Variable(variable.name().c_str()) << ", "
				<< output::Keyword("type") << "(" << output::String(type.name().c_str()) << "))";
		}
		else
		{
			outputStream << output::Function("has") << "("
				<< output::Variable(variable.name().c_str()) << ", "
				<< output::Keyword("type") << "(" << output::String("object") << "))";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
