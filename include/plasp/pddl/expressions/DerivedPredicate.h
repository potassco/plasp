#ifndef __PLASP__PDDL__EXPRESSIONS__DERIVED_PREDICATE_H
#define __PLASP__PDDL__EXPRESSIONS__DERIVED_PREDICATE_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DerivedPredicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class DerivedPredicate: public ExpressionCRTP<DerivedPredicate>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::DerivedPredicate;

		// TODO: consider implementing parsing functions for compatibility with older PDDL versions

	public:
		explicit DerivedPredicate(size_t id);

		size_t id() const;

		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		void print(std::ostream &ostream) const override;

	private:
		size_t m_id;

		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
