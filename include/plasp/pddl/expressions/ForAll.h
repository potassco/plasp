#ifndef __PLASP__PDDL__EXPRESSIONS__FOR_ALL_H
#define __PLASP__PDDL__EXPRESSIONS__FOR_ALL_H

#include <plasp/pddl/expressions/Quantified.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ForAll
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ForAll: public QuantifiedCRTP<ForAll>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::ForAll;

		static const std::string Identifier;

	public:
		ExpressionPointer decomposed(DerivedPredicates &derivedPredicates) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
