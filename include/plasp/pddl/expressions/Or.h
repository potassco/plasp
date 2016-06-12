#ifndef __PLASP__PDDL__EXPRESSIONS__OR_H
#define __PLASP__PDDL__EXPRESSIONS__OR_H

#include <plasp/pddl/expressions/NAry.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Or
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Or: public NAry<Or>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::Or;

		static const std::string Identifier;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
