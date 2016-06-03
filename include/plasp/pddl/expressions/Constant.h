#ifndef __PLASP__PDDL__EXPRESSION__CONSTANT_H
#define __PLASP__PDDL__EXPRESSION__CONSTANT_H

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
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Constant: public Expression
{
	public:
		static ConstantPointer parseDeclaration(utils::Parser &parser, Context &context);
		static void parseTypedDeclaration(utils::Parser &parser, Context &context);

		template<class Container>
		static Constant *parseExisting(utils::Parser &parser, const Container &constants);

		// TODO: method for lazy creation if not existing

	public:
		void accept(ExpressionVisitor &expressionVisitor) const override;

		const std::string &name() const;
		const PrimitiveType *type() const;

		bool isDeclared() const;

	private:
		Constant();

		void setDirty(bool isDirty = true);
		bool isDirty() const;

		void setDeclared();

		void setType(const PrimitiveType *parentType);

		bool m_isDirty;
		bool m_isDeclared;

		std::string m_name;

		const PrimitiveType *m_type;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Container>
Constant *Constant::parseExisting(utils::Parser &parser, const Container &constants)
{
	parser.skipWhiteSpace();

	const auto constantName = parser.parseIdentifier(isIdentifier);
	// TODO: use hash map
	const auto match = std::find_if(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			return constant->name() == constantName;
		});
	const auto constantExists = (match != constants.cend());

	if (!constantExists)
		throw utils::ParserException(parser.row(), parser.column(), "Constant \"" + constantName + "\" used but never declared");

	return match->get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
