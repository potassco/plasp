#ifndef __PDDL__TOKENIZER_H
#define __PDDL__TOKENIZER_H

#include <iostream>

#include <tokenize/Tokenizer.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tokenizer
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PDDLTokenizerPolicy
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
		return c != '?'
			&& c != '('
			&& c != ')'
			&& c != ';'
			&& std::isgraph(c);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using Tokenizer = tokenize::Tokenizer<PDDLTokenizerPolicy>;

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
