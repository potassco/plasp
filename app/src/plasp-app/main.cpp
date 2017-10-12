#include <algorithm>
#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <colorlog/Logger.h>
#include <colorlog/Priority.h>

#include <plasp-app/Commands.h>
#include <plasp-app/Version.h>
#include <plasp-app/commands/Translate.h>

int main(int argc, char **argv)
{
	const auto printHelp =
		[&]()
		{
			// TODO: add list of available commands
			std::cout
				<< "ASP planning tools for PDDL." << std::endl
				<< "Usage: plasp <command> [<arguments>]" << std::endl
				<< "Translate PDDL to ASP." << std::endl;
		};

	const auto printVersion =
		[&]()
		{
			std::cout << Version << std::endl;
		};

	colorlog::Logger logger;

	if (argc < 2)
	{
		printHelp();
		return EXIT_FAILURE;
	}

	try
	{
		switch (parseCommand(argv[1]))
		{
			case Command::Help:
				printHelp();
				return EXIT_SUCCESS;
			case Command::Version:
				printVersion();
				return EXIT_SUCCESS;
			case Command::Translate:
				return translate(argc - 1, &argv[1]);
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
