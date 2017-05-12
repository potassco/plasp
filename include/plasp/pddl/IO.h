#ifndef __PLASP__PDDL__IO_H
#define __PLASP__PDDL__IO_H

#include <iostream>

#include <plasp/pddl/Tokenizer.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IO
//
////////////////////////////////////////////////////////////////////////////////////////////////////

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
