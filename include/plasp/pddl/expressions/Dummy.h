#ifndef __PLASP__PDDL__EXPRESSIONS__DUMMY_H
#define __PLASP__PDDL__EXPRESSIONS__DUMMY_H

#include <plasp/pddl/Expression.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Dummy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Dummy: public ExpressionCRTP<Dummy>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::Dummy;

	public:
		Dummy(std::string name);

		void print(std::ostream &ostream) const override;

	private:
		std::string m_name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
