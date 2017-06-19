#ifndef __PLASP__PDDL__TRANSLATION__VARIABLES_H
#define __PLASP__PDDL__TRANSLATION__VARIABLES_H

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>

namespace plasp
{
namespace pddl
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

	for (const auto &variable : variables)
		outputStream << ", " << *variable;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline void translateVariablesForRuleBody(output::ColorStream &outputStream, const T &variables)
{
	if (variables.empty())
		return;

	outputStream << " :- ";

	for (const auto &variable : variables)
	{
		if (variable.get() != variables.begin()->get())
			outputStream << ", ";

		if (variable->type)
		{
			if (!variable->type.value().template is<::pddl::ast::PrimitiveTypePointer>())
				throw output::TranslatorException("only primitive types supported currently");

			const auto &type = variable->type.value().template get<::pddl::ast::PrimitiveTypePointer>();

			outputStream << output::Function("has") << "("
				<< *variable << ", " << output::Keyword("type") << "(" << *type << "))";
		}
		else
		{
			outputStream << output::Function("has") << "("
				<< *variable << ", "
				<< output::Keyword("type") << "(" << output::String("object") << "))";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
