#ifndef __PLASP__PDDL__EXPRESSIONS__PRIMITIVE_TYPE_H
#define __PLASP__PDDL__EXPRESSIONS__PRIMITIVE_TYPE_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Identifier.h>
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

class PrimitiveType: public Expression
{
	public:
		static void parseDeclaration(Context &context, Domain &domain);
		static void parseTypedDeclaration(Context &context, Domain &domain);

		static PrimitiveType *parseAndFind(Context &context, Domain &domain);

	public:
		PrimitiveType();
		PrimitiveType(std::string name);

		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const std::vector<const PrimitiveType *> &parentTypes() const;

	private:
		void setDirty(bool isDirty = true);
		bool isDirty() const;

		bool m_isDirty;

		std::string m_name;

		std::vector<const PrimitiveType *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
