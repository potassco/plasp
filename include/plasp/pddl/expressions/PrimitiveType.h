#ifndef __PLASP__PDDL__EXPRESSIONS__PRIMITIVE_TYPE_H
#define __PLASP__PDDL__EXPRESSIONS__PRIMITIVE_TYPE_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Expression.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrimitiveType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PrimitiveType: public ExpressionCRTP<PrimitiveType>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::PrimitiveType;

		static void parseDeclaration(Context &context, Domain &domain);
		static void parseTypedDeclaration(Context &context, Domain &domain);

		static PrimitiveTypePointer parseAndFind(Context &context, Domain &domain);

	public:
		PrimitiveType();
		PrimitiveType(std::string name);

		const std::string &name() const;
		const PrimitiveTypes &parentTypes() const;

		void print(std::ostream &ostream) const override;

	private:
		void setDirty(bool isDirty = true);
		bool isDirty() const;

		bool m_isDirty;

		std::string m_name;

		PrimitiveTypes m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
