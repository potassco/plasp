#ifndef __PLASP__PDDL__EXPRESSIONS__DERIVED_PREDICATE_H
#define __PLASP__PDDL__EXPRESSIONS__DERIVED_PREDICATE_H

#include <set>

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
		void setPreconditions(std::vector<Expressions> &&preconditions);
		const std::vector<Expressions> &preconditions() const;

		const std::set<VariablePointer> &parameters() const;

		void collectParameters(std::set<VariablePointer> &parameters) override;

		void print(std::ostream &ostream) const override;

	private:
		void collectParameters();

		// The arguments are interpreted as a disjunction of conjunctions
		std::vector<Expressions> m_preconditions;

		std::set<VariablePointer> m_parameters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
