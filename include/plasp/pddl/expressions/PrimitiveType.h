#ifndef __PLASP__PDDL__EXPRESSION__PRIMITIVE_TYPE_H
#define __PLASP__PDDL__EXPRESSION__PRIMITIVE_TYPE_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Identifier.h>
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
// PrimitiveType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class PrimitiveType: public Expression
{
	public:
		static PrimitiveTypePointer parseDeclaration(utils::Parser &parser, Context &context);
		static void parseTypedDeclaration(utils::Parser &parser, Context &context);

		template<class Container>
		static PrimitiveType *parseExisting(utils::Parser &parser, const Container &primitiveTypes);

		// TODO: method for lazy creation if not existing

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const std::vector<const PrimitiveType *> &parentTypes() const;

		bool isDeclared() const;

	private:
		PrimitiveType();

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setDeclared();

		void addParentType(const PrimitiveType *parentType);

		bool m_isDirty;
		bool m_isDeclared;

		std::string m_name;

		std::vector<const PrimitiveType *> m_parentTypes;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Container>
PrimitiveType *PrimitiveType::parseExisting(utils::Parser &parser,
	const Container &primitiveTypes)
{
	parser.skipWhiteSpace();

	const auto typeName = parser.parseIdentifier(isIdentifier);
	// TODO: use hash map
	const auto match = std::find_if(primitiveTypes.cbegin(), primitiveTypes.cend(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name() == typeName;
		});
	const auto typeExists = (match != primitiveTypes.cend());

	// Return existing primitive types
	if (!typeExists)
		throw utils::ParserException(parser.row(), parser.column(), "Primitive type \"" + typeName + "\" used but never declared");

	auto *type = match->get();
	type->setDirty();

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
