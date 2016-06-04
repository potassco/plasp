#include <plasp/pddl/expressions/Constant.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/expressions/PrimitiveType.h>

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

Constant::Constant()
:	m_isDirty{false},
	m_isDeclared{false},
	m_type{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ConstantPointer Constant::parseDeclaration(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	auto constant = std::make_unique<Constant>(Constant());

	constant->m_name = parser.parseIdentifier(isIdentifier);

	BOOST_ASSERT(constant->m_name != "-");

	// Flag constant for potentially upcoming type declaration
	constant->setDirty();

	// TODO: Store constant in hash map

	return constant;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::parseTypedDeclaration(utils::Parser &parser, Context &context)
{
	// Parse and store constant
	context.constants.emplace_back(parseDeclaration(parser, context));

	const auto &constant = context.constants.back();

	// Flag constant as correctly declared in the types section
	constant->setDeclared();

	parser.skipWhiteSpace();

	// Check for typing information
	if (!parser.advanceIf('-'))
		return;

	// If existing, parse and store parent type
	auto *type = PrimitiveType::parseExisting(parser, context.primitiveTypes);

	// Assign parent type to all types that were previously flagged
	std::for_each(context.constants.begin(), context.constants.end(),
		[&](auto &constant)
		{
			if (!constant->isDirty())
				return;

			constant->setType(type);
			constant->setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Constant::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Constant::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Constant::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::setType(const PrimitiveType *type)
{
	m_type = type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const PrimitiveType *Constant::type() const
{
	return m_type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
