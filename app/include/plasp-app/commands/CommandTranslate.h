#ifndef __PLASP_APP__COMMANDS__COMMAND_TRANSLATE_H
#define __PLASP_APP__COMMANDS__COMMAND_TRANSLATE_H

#include <plasp-app/Command.h>
#include <plasp-app/OptionGroups.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Translate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class CommandTranslate : public Command<CommandTranslate, OptionGroupBasic, OptionGroupOutput, OptionGroupParser>
{
	public:
		static constexpr auto Name = "translate";
		static constexpr auto Description = "Translate PDDL and SAS to ASP facts";

	public:
		int run(int argc, char **argv);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
