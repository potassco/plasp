#include <plasp-app/Commands.h>

#include <map>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Commands
//
////////////////////////////////////////////////////////////////////////////////////////////////////

static const std::map<std::string, Command> commandNames =
	{
		{"help", Command::Help},
		{"-h", Command::Help},
		{"--help", Command::Help},
		{"version", Command::Version},
		{"-v", Command::Version},
		{"--version", Command::Version},
		{"check-syntax", Command::CheckSyntax},
		{"requirements", Command::Requirements},
		{"pretty-print", Command::PrettyPrint},
		{"normalize", Command::Normalize},
		{"translate", Command::Translate},
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

Command parseCommand(const std::string &commandString)
{
	const auto matchingCommand = commandNames.find(commandString);

	if (matchingCommand == commandNames.cend())
		throw std::runtime_error(std::string("“") + commandString + "” is not a plasp command");

	return matchingCommand->second;
}
