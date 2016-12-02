#ifndef __PLASP__INPUT__PARSER_POLICY_H
#define __PLASP__INPUT__PARSER_POLICY_H

#include <iostream>

namespace plasp
{
namespace input
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserPolicy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class CaseSensitiveParserPolicy
{
	public:
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

class CaseInsensitiveParserPolicy
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
			return std::isgraph(c);
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
