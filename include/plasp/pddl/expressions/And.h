#ifndef __PLASP__PDDL__EXPRESSIONS__AND_H
#define __PLASP__PDDL__EXPRESSIONS__AND_H

#include <plasp/pddl/expressions/NAry.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// And
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class And: public NAry<And>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::And;

		static const std::string Identifier;

	public:
		ExpressionPointer disjunctionNormalized() override;
		ExpressionPointer decomposed(DerivedPredicates &derivedPredicates) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
