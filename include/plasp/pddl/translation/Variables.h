#ifndef __PLASP__PDDL__TRANSLATION__VARIABLES_H
#define __PLASP__PDDL__TRANSLATION__VARIABLES_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>
#include <pddl/Parse.h>

#include <plasp/TranslatorException.h>
#include <plasp/pddl/translation/Variable.h>

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
void translateVariablesForRuleHead(colorlog::ColorStream &outputStream, const T &variables, VariableIDMap &variableIDs);
template<class T>
void translateVariablesForRuleBody(colorlog::ColorStream &outputStream, const T &variables, VariableIDMap &variableIDs);

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline void translateVariablesForRuleHead(colorlog::ColorStream &outputStream, const T &variables, VariableIDMap &variableIDs)
{
	for (const auto &variable : variables)
	{
		outputStream << ", ";
		translateVariableDeclaration(outputStream, *variable, variableIDs);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
void translateVariablesForRuleBody(colorlog::ColorStream &outputStream, const T &variables, VariableIDMap &variableIDs)
{
	for (const auto &variable : variables)
	{
		if (&variable != &*variables.begin())
			outputStream << ", ";

		if (variable->type)
		{
			if (!variable->type.value().template is<::pddl::normalizedAST::PrimitiveTypePointer>())
				throw TranslatorException("only primitive types supported currently");

			const auto &type = variable->type.value().template get<::pddl::normalizedAST::PrimitiveTypePointer>();

			outputStream << colorlog::Function("has") << "(";
			translateVariableDeclaration(outputStream, *variable, variableIDs);
			outputStream << ", " << colorlog::Keyword("type") << "(" << *type << "))";
		}
		else
		{
			outputStream << colorlog::Function("has") << "(";
			translateVariableDeclaration(outputStream, *variable, variableIDs);
			outputStream << ", " << colorlog::Keyword("type") << "(" << colorlog::String("object") << "))";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
