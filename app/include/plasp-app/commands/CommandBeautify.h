#ifndef __PLASP_APP__COMMANDS__COMMAND_BEAUTIFY_H
#define __PLASP_APP__COMMANDS__COMMAND_BEAUTIFY_H

#include <plasp-app/Command.h>
#include <plasp-app/OptionGroups.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Beautify
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class CommandBeautify : public Command<CommandBeautify, OptionGroupBasic, OptionGroupOutput, OptionGroupParser>
{
	public:
		static constexpr auto Name = "beautify";
		static constexpr auto Description = "Cleanly format PDDL specifications";

	public:
		int run(int argc, char **argv);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
