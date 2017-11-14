#include <plasp-app/commands/CommandHelp.h>

#include <colorlog/Logger.h>

#include <plasp-app/CommandType.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Help
//
////////////////////////////////////////////////////////////////////////////////////////////////////

int CommandHelp::run(int argc, char **argv)
{
	colorlog::Logger logger;

	if (argc > 1)
	{
		try
		{
			switch (parseCommandType(argv[1]))
			{
				case CommandType::Help:
				case CommandType::Version:
					break;

				case CommandType::Translate:
					CommandTranslate().printHelp();
					return EXIT_SUCCESS;

				case CommandType::Normalize:
					CommandNormalize().printHelp();
					return EXIT_SUCCESS;

				case CommandType::Beautify:
					CommandBeautify().printHelp();
					return EXIT_SUCCESS;

				case CommandType::CheckSyntax:
					CommandCheckSyntax().printHelp();
					return EXIT_SUCCESS;

				default:
					logger.log(colorlog::Priority::Error, std::string("command “") + argv[1] + "” not yet implemented");
					exit(EXIT_FAILURE);
			}
		}
		catch (std::exception &exception)
		{
		}
	}

	std::cout
		<< "ASP planning tools for PDDL." << std::endl
		<< "Usage:" << std::endl
		<< "  plasp <command> [<arguments>]" << std::endl << std::endl
		<< " available commands:" << std::endl;

	AvailableCommands availableCommands;

	size_t lengthOfLongestCommandName = 0;

	forEach(availableCommands,
		[&](const auto &command)
		{
			lengthOfLongestCommandName = std::max(lengthOfLongestCommandName, std::strlen(command.Name));
		});

	const auto printSpaces =
		[](const auto numberOfSpaces)
		{
			for (auto i = static_cast<decltype(numberOfSpaces)>(0); i < numberOfSpaces; i++)
				std::cout << " ";
		};

	forEach(availableCommands,
		[&](const auto &command)
		{
			const auto lengthOfCommandName = std::strlen(command.Name);

			std::cout << "  " << command.Name;
			printSpaces(lengthOfLongestCommandName - lengthOfCommandName + 2);
			std::cout << command.Description << std::endl;
		});

	return EXIT_SUCCESS;
}
