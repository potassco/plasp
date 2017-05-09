#ifndef __PDDL_PARSE__DETAIL__PARSER_H
#define __PDDL_PARSE__DETAIL__PARSER_H

#include <iostream>

#include <parsebase/Parser.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parser
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PDDLParserPolicy
{
	public:
		static char transformCharacter(char c) noexcept
		{
			return std::tolower(c);
		}

		static bool isWhiteSpaceCharacter(char c)
		{
			return std::iswspace(c);
		}

		static bool isBlankCharacter(char c)
		{
			return std::isblank(c);
		}

		static bool isIdentifierCharacter(char c)
		{
			return c != '?'
				&& c != '('
				&& c != ')'
				&& c != ';'
				&& std::isgraph(c);
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using Parser = parsebase::Parser<PDDLParserPolicy>;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
