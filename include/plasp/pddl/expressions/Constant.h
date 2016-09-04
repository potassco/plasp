#ifndef __PLASP__PDDL__EXPRESSIONS__CONSTANT_H
#define __PLASP__PDDL__EXPRESSIONS__CONSTANT_H

#include <plasp/pddl/Expression.h>
#include <plasp/utils/Parser.h>
#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Constant: public ExpressionCRTP<Constant>
{
	public:
		static const Expression::Type ExpressionType = Expression::Type::Constant;

		static void parseTypedDeclaration(Context &context, Domain &domain);
		static void parseTypedDeclarations(Context &context, Domain &domain);
		static void parseTypedDeclaration(Context &context, Problem &problem);
		static void parseTypedDeclarations(Context &context, Problem &problem);

		static ConstantPointer parseAndFind(Context &context, const Domain &domain);
		static ConstantPointer parseAndFind(Context &context, const Problem &problem);

	public:
		const std::string &name() const;
		PrimitiveTypePointer type() const;

		ExpressionPointer normalize() override;

	private:
		static ConstantPointer parseDeclaration(Context &context);
		static void parseTypedDeclaration(Context &context, Domain &domain, Constants &constants);

		static ConstantPointer parseAndFind(const std::string &constantName, const Constants &constants);

		Constant();

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setType(PrimitiveTypePointer parentType);

		bool m_isDirty;

		std::string m_name;

		PrimitiveTypePointer m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
