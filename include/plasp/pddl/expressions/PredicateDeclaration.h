#ifndef __PLASP__PDDL__EXPRESSIONS__PREDICATE_DECLARATION_H
#define __PLASP__PDDL__EXPRESSIONS__PREDICATE_DECLARATION_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PredicateDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PredicateDeclaration: public ExpressionCRTP<PredicateDeclaration>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::PredicateDeclaration;

		static void parse(Context &context, Domain &domain);

	public:
		const std::string &name() const;
		const Variables &arguments() const;

		bool isDeclared() const;

		void normalizeParameterNames();

	private:
		PredicateDeclaration();

		void setDeclared();

		bool m_isDeclared;

		std::string m_name;
		Variables m_parameters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
