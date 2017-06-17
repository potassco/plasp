#ifndef __PDDL_PARSE__DETAIL__PARSE_UTILS_H
#define __PDDL_PARSE__DETAIL__PARSE_UTILS_H

#include <pddlparse/Tokenizer.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParseUtils
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: refactor
inline void skipSection(Tokenizer &tokenizer)
{
	size_t openParentheses = 1;

	while (true)
	{
		const auto character = tokenizer.currentCharacter();
		tokenizer.advance();

		if (character == '(')
			openParentheses++;
		else if (character == ')')
		{
			openParentheses--;

			if (openParentheses == 0)
				return;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
