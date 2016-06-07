#ifndef __PLASP__PDDL__IO_H
#define __PLASP__PDDL__IO_H

#include <iostream>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IO
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const auto skipSection =
	[](utils::Parser &parser)
	{
		size_t openParentheses = 1;

		while (true)
		{
			const auto character = parser.currentCharacter();
			parser.advance();

			if (character == '(')
				openParentheses++;
			else if (character == ')')
			{
				openParentheses--;

				if (openParentheses == 0)
					return;
			}
		}
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
