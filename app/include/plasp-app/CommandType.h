#ifndef __PLASP_APP__COMMAND_TYPE_H
#define __PLASP_APP__COMMAND_TYPE_H

#include <map>
#include <string>

#include <plasp-app/commands/CommandBeautify.h>
#include <plasp-app/commands/CommandCheckSyntax.h>
#include <plasp-app/commands/CommandHelp.h>
#include <plasp-app/commands/CommandNormalize.h>
#include <plasp-app/commands/CommandTranslate.h>
#include <plasp-app/commands/CommandVersion.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class CommandType
{
	Help,
	Version,
	CheckSyntax,
	Requirements,
	Beautify,
	Normalize,
	Translate
};

////////////////////////////////////////////////////////////////////////////////////////////////////

static const std::map<std::string, CommandType> commandNames =
	{
		{"help", CommandType::Help},
		{"-h", CommandType::Help},
		{"--help", CommandType::Help},
		{"version", CommandType::Version},
		{"-v", CommandType::Version},
		{"--version", CommandType::Version},
		{"check-syntax", CommandType::CheckSyntax},
		{"requirements", CommandType::Requirements},
		{"beautify", CommandType::Beautify},
		{"normalize", CommandType::Normalize},
		{"translate", CommandType::Translate},
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

const auto parseCommandType =
	[](const std::string &commandString)
	{
		const auto matchingCommand = commandNames.find(commandString);

		if (matchingCommand == commandNames.cend())
			throw std::runtime_error(std::string("“") + commandString + "” is not a plasp command");

		return matchingCommand->second;
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

using AvailableCommands = std::tuple<CommandTranslate, CommandNormalize, CommandCheckSyntax, CommandBeautify, CommandHelp, CommandVersion>;

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
