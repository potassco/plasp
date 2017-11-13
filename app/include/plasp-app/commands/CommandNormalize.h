#ifndef __PLASP_APP__COMMANDS__COMMAND_NORMALIZE_H
#define __PLASP_APP__COMMANDS__COMMAND_NORMALIZE_H

#include <plasp-app/Command.h>
#include <plasp-app/OptionGroups.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Normalize
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class CommandNormalize : public Command<CommandNormalize, OptionGroupBasic, OptionGroupOutput, OptionGroupParser>
{
	public:
		static constexpr auto Name = "normalize";
		static constexpr auto Description = "Normalize PDDL to plasp’s custom PDDL format";

	public:
		int run(int argc, char **argv);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
