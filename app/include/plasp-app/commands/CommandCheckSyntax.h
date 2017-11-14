#ifndef __PLASP_APP__COMMANDS__COMMAND_CHECK_SYNTAX_H
#define __PLASP_APP__COMMANDS__COMMAND_CHECK_SYNTAX_H

#include <plasp-app/Command.h>
#include <plasp-app/OptionGroups.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Check Syntax
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class CommandCheckSyntax : public Command<CommandCheckSyntax, OptionGroupBasic, OptionGroupOutput, OptionGroupParser>
{
	public:
		static constexpr auto Name = "check-syntax";
		static constexpr auto Description = "Check the syntax of PDDL specifications";

	public:
		int run(int argc, char **argv);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
