#ifndef __PLASP__PDDL__TOKENIZER_H
#define __PLASP__PDDL__TOKENIZER_H

#include <tokenize/Tokenizer.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tokenizer
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PDDLTokenizerPolicy
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

using Tokenizer = tokenize::Tokenizer<PDDLTokenizerPolicy>;

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
