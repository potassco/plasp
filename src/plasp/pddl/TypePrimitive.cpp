#include <plasp/pddl/TypePrimitive.h>

#include <algorithm>

#include <boost/variant.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TypePrimitive
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TypePrimitive::TypePrimitive(std::string name)
:	m_isDirty{false},
	m_isDeclared{false},
	m_name(name)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TypePrimitive &TypePrimitive::parse(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	const auto typeName = parser.parseIdentifier(isIdentifier);
	const auto match = context.types.find(typeName);
	const auto typeExists = (match != context.types.cend());

	if (typeExists)
	{
		auto &type = match->second;
		auto &primitiveType = boost::get<TypePrimitive>(type);

		primitiveType.setDirty();

		return primitiveType;
	}

	const auto insertionResult = context.types.emplace(std::make_pair(typeName, TypePrimitive(typeName)));
	auto &type = insertionResult.first->second;
	auto &primitiveType = boost::get<TypePrimitive>(type);

	// Flag type for potentially upcoming parent type declaration
	primitiveType.setDirty();

	return primitiveType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TypePrimitive &TypePrimitive::parseDeclaration(utils::Parser &parser, Context &context)
{
	// Parse and store type
	auto &type = parse(parser, context);

	// Flag type as correctly declared in the types section
	type.setDeclared();

	parser.skipWhiteSpace();

	// Check for type inheritance
	if (!parser.advanceIf('-'))
		return type;

	// If existing, parse and store parent type
	auto &parentType = parse(parser, context);

	parentType.setDirty(false);

	// Type object is an implicit primitive type
	if (parentType.name() == "object")
		parentType.setDeclared();

	// Assign parent type to all types that were previously flagged
	std::for_each(context.types.begin(), context.types.end(),
		[&](auto &childType)
		{
			auto &childTypePrimitive = boost::get<TypePrimitive>(childType.second);

			if (!childTypePrimitive.isDirty())
				return;

			childTypePrimitive.addParentType(parentType);
			childTypePrimitive.setDirty(false);
		});

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TypePrimitive::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TypePrimitive::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TypePrimitive::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool TypePrimitive::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &TypePrimitive::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TypePrimitive::addParentType(const TypePrimitive &parentType)
{
	m_parentTypes.emplace_back(&parentType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const TypePrimitive *> &TypePrimitive::parentTypes() const
{
	return m_parentTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
