#include <plasp/pddl/Type.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Type
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Type::Type(std::string name)
:	m_isDirty{false},
	m_isDeclared{false},
	m_name(name)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Type::parsePDDL(utils::Parser &parser, Context &context)
{
	// Parses a single type identifier
	const auto parseType =
		[&]() -> auto &
		{
			parser.skipWhiteSpace();

			const auto typeName = parser.parseIdentifier(isIdentifier);
			const auto match = context.types.find(typeName);
			const auto typeExists = (match != context.types.cend());

			if (typeExists)
			{
				auto &type = match->second;

				type.setDirty();

				return type;
			}

			const auto insertionResult = context.types.emplace(std::make_pair(typeName, Type(typeName)));
			auto &type = insertionResult.first->second;

			// Flag type for potentially upcoming parent type declaration
			type.setDirty();

			// Flag type as correctly declared in the types section
			type.setDeclared();

			return type;
		};

	parseType();

	parser.skipWhiteSpace();

	// Check for type inheritance
	if (!parser.advanceIf('-'))
		return;

	// If existing, parse parent type
	auto &parentType = parseType();

	parentType.setDirty(false);

	// Assign parent type to all types that were previously flagged
	std::for_each(context.types.begin(), context.types.end(),
		[&](auto &childType)
		{
			if (!childType.second.isDirty())
				return;

			childType.second.addParentType(parentType);
			childType.second.setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Type::setDirty(bool isDirty)
{
	m_isDirty = isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Type::isDirty() const
{
	return m_isDirty;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Type::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Type::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Type::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Type::addParentType(const Type &parentType)
{
	m_parentTypes.emplace_back(&parentType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const Type *> &Type::parentTypes() const
{
	return m_parentTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
