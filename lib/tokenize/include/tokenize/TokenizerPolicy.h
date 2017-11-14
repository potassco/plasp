#ifndef __TOKENIZE__TOKENIZER_POLICY_H
#define __TOKENIZE__TOKENIZER_POLICY_H

#include <iostream>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TokenizerPolicy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CaseSensitiveTokenizerPolicy
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

struct CaseInsensitiveTokenizerPolicy
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
