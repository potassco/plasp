#include <plasp/pddl/PrimitiveType.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// PrimitiveType
//
////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType::PrimitiveType(std::string name)
:	m_isDirty{false},
	m_isDeclared{false},
	m_name(name)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType &PrimitiveType::parse(utils::Parser &parser, Context &context)
{
	parser.skipWhiteSpace();

	const auto typeName = parser.parseIdentifier(isIdentifier);
	const auto match = context.primitiveTypesHashMap.find(typeName);
	const auto typeExists = (match != context.primitiveTypesHashMap.cend());

	// Return existing primitive types
	if (typeExists)
	{
		auto &type = *match->second;

		type.setDirty();

		return type;
	}

	// Store new primitive type
	context.primitiveTypes.emplace_back(std::make_unique<PrimitiveType>(PrimitiveType(typeName)));

	auto &type = *context.primitiveTypes.back();

	// Add a pointer to the primitive type to the hash map
	context.primitiveTypesHashMap.emplace(std::make_pair(typeName, &type));

	// Flag type for potentially upcoming parent type declaration
	type.setDirty();

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType &PrimitiveType::parseDeclaration(utils::Parser &parser, Context &context)
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
	std::for_each(context.primitiveTypes.begin(), context.primitiveTypes.end(),
		[&](auto &childType)
		{
			if (!childType->isDirty())
				return;

			childType->addParentType(parentType);
			childType->setDirty(false);
		});

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrimitiveType::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool PrimitiveType::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &PrimitiveType::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::addParentType(const PrimitiveType &parentType)
{
	m_parentTypes.emplace_back(&parentType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const PrimitiveType *> &PrimitiveType::parentTypes() const
{
	return m_parentTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
