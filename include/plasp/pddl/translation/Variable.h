#ifndef __PLASP__PDDL__TRANSLATION__VARIABLE_H
#define __PLASP__PDDL__TRANSLATION__VARIABLE_H

#include <map>

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

using VariableIDMap = std::map<const ::pddl::normalizedAST::VariableDeclaration *, size_t>;

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateVariableDeclaration(colorlog::ColorStream &stream, ::pddl::normalizedAST::VariableDeclaration &variableDeclaration, VariableIDMap &variableIDs)
{
	assert(!variableDeclaration.name.empty());

	const auto variableID =
		[&]()
		{
			const auto matchingVariableID = variableIDs.find(&variableDeclaration);

			if (matchingVariableID != variableIDs.cend())
				return matchingVariableID->second;

			const auto variableID = variableIDs.size() + 1;

			variableIDs.insert({&variableDeclaration, variableID});

			return variableID;
		};

	stream
		<< colorlog::Format({colorlog::Color::Green, colorlog::FontWeight::Bold})
		<< "X" << variableID()
		<< colorlog::ResetFormat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void translateVariable(colorlog::ColorStream &stream, ::pddl::normalizedAST::Variable &variable, VariableIDMap &variableIDs)
{
	assert(variable.declaration != nullptr);

	translateVariableDeclaration(stream, *variable.declaration, variableIDs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
