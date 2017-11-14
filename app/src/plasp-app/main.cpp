#include <algorithm>
#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <colorlog/Logger.h>
#include <colorlog/Priority.h>

#include <plasp-app/Command.h>
#include <plasp-app/CommandType.h>
#include <plasp-app/Version.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Main
//
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	colorlog::Logger logger;

	if (argc < 2)
	{
		CommandHelp().run(argc - 1, &argv[1]);

		return EXIT_FAILURE;
	}

	try
	{
		switch (parseCommandType(argv[1]))
		{
			case CommandType::Help:
				return CommandHelp().run(argc - 1, &argv[1]);

			case CommandType::Version:
				return CommandVersion().run(argc - 1, &argv[1]);

			case CommandType::Translate:
				return CommandTranslate().run(argc - 1, &argv[1]);

			case CommandType::Normalize:
				return CommandNormalize().run(argc - 1, &argv[1]);

			case CommandType::Beautify:
				return CommandBeautify().run(argc - 1, &argv[1]);

			case CommandType::CheckSyntax:
				return CommandCheckSyntax().run(argc - 1, &argv[1]);

			default:
				logger.log(colorlog::Priority::Error, std::string("command “") + argv[1] + "” not yet implemented");
				exit(EXIT_FAILURE);
		}
	}
	catch (std::exception &exception)
	{
		logger.log(colorlog::Priority::Error, exception.what());
		std::cout << std::endl;

		CommandHelp().run(argc - 1, &argv[1]);

		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
