#include <plasp/pddl/expressions/PrimitiveType.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>

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

PrimitiveType::PrimitiveType()
:	m_isDirty{true}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType::PrimitiveType(std::string name)
:	m_isDirty{true},
	m_name{name}
{
	BOOST_ASSERT(!m_name.empty());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::parseDeclaration(Context &context, Domain &domain)
{
	auto &types = domain.types();

	context.parser.skipWhiteSpace();

	const auto typeName = context.parser.parseIdentifier(isIdentifier);

	const auto match = std::find_if(types.cbegin(), types.cend(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name() == typeName;
		});

	// Return existing primitive type
	if (match != types.cend())
	{
		auto *type = match->get();

		type->setDirty();

		return;
	}

	types.emplace_back(std::make_unique<PrimitiveType>(typeName));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::parseTypedDeclaration(Context &context, Domain &domain)
{
	auto &types = domain.types();

	// Parse and store type
	parseDeclaration(context, domain);

	context.parser.skipWhiteSpace();

	// Check for type inheritance
	if (!context.parser.probe('-'))
		return;

	domain.checkRequirement(Requirement::Type::Typing);

	// If existing, parse and store parent type
	auto *parentType = parseAndFind(context, domain);

	parentType->setDirty(false);

	// Assign parent type to all types that were previously flagged
	std::for_each(types.begin(), types.end(),
		[&](auto &childType)
		{
			if (!childType->isDirty())
				return;

			childType->m_parentTypes.push_back(parentType);
			childType->setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType *PrimitiveType::parseAndFind(Context &context, Domain &domain)
{
	auto &types = domain.types();

	context.parser.skipWhiteSpace();

	const auto typeName = context.parser.parseIdentifier(isIdentifier);

	if (typeName.empty())
		throw utils::ParserException(context.parser, "no type supplied");

	const auto match = std::find_if(types.cbegin(), types.cend(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name() == typeName;
		});

	if (match == types.cend())
	{
		// Only "object" is allowed as an implicit type
		if (typeName == "object" || typeName == "objects")
		{
			context.logger.logWarning(context.parser, "primitive type “" + typeName + "” should be declared");
			types.emplace_back(std::make_unique<expressions::PrimitiveType>(typeName));
		}
		else
			throw utils::ParserException(context.parser, "type “" + typeName + "” used but never declared");

		return types.back().get();
	}

	auto *type = match->get();
	type->setDirty();

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

const std::string &PrimitiveType::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::vector<const PrimitiveType *> &PrimitiveType::parentTypes() const
{
	return m_parentTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
