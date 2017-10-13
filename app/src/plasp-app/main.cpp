#include <algorithm>
#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <colorlog/Logger.h>
#include <colorlog/Priority.h>

#include <plasp-app/Command.h>
#include <plasp-app/Version.h>
#include <plasp-app/commands/CommandNormalize.h>
#include <plasp-app/commands/CommandTranslate.h>
#include <plasp-app/commands/CommandVersion.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Main
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class CommandType
{
	Help,
	Version,
	CheckSyntax,
	Requirements,
	PrettyPrint,
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
		{"pretty-print", CommandType::PrettyPrint},
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

int main(int argc, char **argv)
{
	const auto printHelp =
		[&]()
		{
			// TODO: add list of available commands
			std::cout
				<< "ASP planning tools for PDDL." << std::endl
				<< "Usage: plasp <command> [<arguments>]" << std::endl;
		};

	colorlog::Logger logger;

	if (argc < 2)
	{
		printHelp();
		return EXIT_FAILURE;
	}

	try
	{
		switch (parseCommandType(argv[1]))
		{
			case CommandType::Help:
				printHelp();
				return EXIT_SUCCESS;
			case CommandType::Version:
				return CommandVersion().run(argc - 1, &argv[1]);
				return EXIT_SUCCESS;
			case CommandType::Normalize:
				return CommandNormalize().run(argc - 1, &argv[1]);
			case CommandType::Translate:
				return CommandTranslate().run(argc - 1, &argv[1]);
			default:
				exit(EXIT_FAILURE);
		}
	}
	catch (std::exception &exception)
	{
		logger.log(colorlog::Priority::Error, exception.what());
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
