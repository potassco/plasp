#include <plasp/pddl/Constant.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Constant::Constant(std::string name)
:	m_isDirty{false},
	m_isDeclared{false},
	m_name(name),
	m_type{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Constant &Constant::parse(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	const auto constantName = parser.parseIdentifier(isIdentifier);
	const auto match = context.constantsHashMap.find(constantName);
	const auto constantExists = (match != context.constantsHashMap.cend());

	// Return existing primitive types
	if (constantExists)
	{
		auto &constant = *match->second;

		constant.setDirty();

		return constant;
	}

	// Store new primitive type
	context.constants.emplace_back(std::make_unique<Constant>(Constant(constantName)));

	auto &constant = *context.constants.back();

	// Add a pointer to the primitive type to the hash map
	context.constantsHashMap.emplace(std::make_pair(constantName, &constant));

	// Flag type for potentially upcoming parent type declaration
	constant.setDirty();

	return constant;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Constant &Constant::parseDeclaration(utils::Parser &parser, Context &context)
{
	// Parse and store constant
	auto &constant = parse(parser, context);

	// Flag constant as correctly declared in the types section
	constant.setDeclared();

	parser.skipWhiteSpace();

	// Check for typing information
	if (!parser.advanceIf('-'))
		return constant;

	// If existing, parse and store parent type
	auto &type = PrimitiveType::parse(parser, context);

	// Assign parent type to all types that were previously flagged
	std::for_each(context.constants.begin(), context.constants.end(),
		[&](auto &constant)
		{
			if (!constant->isDirty())
				return;

			constant->setType(&type);
			constant->setDirty(false);
		});

	return constant;
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
