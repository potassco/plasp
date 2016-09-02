#ifndef __PLASP__PDDL__EXPRESSIONS__UNSUPPORTED_H
#define __PLASP__PDDL__EXPRESSIONS__UNSUPPORTED_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Unsupported
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Unsupported: public ExpressionCRTP<Unsupported>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::Unsupported;

		static UnsupportedPointer parse(Context &context);

	public:
		const std::string &type() const;

		ExpressionPointer normalize() override;

	private:
		std::string m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
