#ifndef __PARSE_BASE__PARSER_POLICY_H
#define __PARSE_BASE__PARSER_POLICY_H

#include <iostream>

namespace parsebase
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserPolicy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CaseSensitiveParserPolicy
{
	static constexpr char transformCharacter(char c) noexcept
	{
		return c;
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
		return std::isgraph(c);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct CaseInsensitiveParserPolicy
{
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
		return std::isgraph(c);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
