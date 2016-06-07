#include <plasp/pddl/expressions/PrimitiveType.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/ExpressionVisitor.h>

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
:	m_isDirty{true},
	m_isDeclared{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveType::PrimitiveType(std::string name)
:	m_isDirty{true},
	m_isDeclared{false},
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

	// TODO: use hash map
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
		type->setDeclared();

		return;
	}

	types.emplace_back(std::make_unique<PrimitiveType>(typeName));
	types.back()->setDeclared();
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

	if (!domain.hasRequirement(Requirement::Type::Typing))
		throw utils::ParserException(context.parser, "Typing used but not declared as a requirement");

	// If existing, parse and store parent type
	auto *parentType = parseAndFindOrCreate(context, domain);

	parentType->setDirty(false);

	// Flag parent tpe as correctly declared in the types section
	parentType->setDeclared();

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

PrimitiveType *PrimitiveType::parseAndFindOrCreate(Context &context, Domain &domain)
{
	auto &types = domain.types();

	context.parser.skipWhiteSpace();

	const auto typeName = context.parser.parseIdentifier(isIdentifier);

	BOOST_ASSERT(!typeName.empty());

	// TODO: use hash map
	const auto match = std::find_if(types.cbegin(), types.cend(),
		[&](const auto &primitiveType)
		{
			return primitiveType->name() == typeName;
		});

	if (match == types.cend())
	{
		// If necessary, insert new primitive type but don't declare it
		types.emplace_back(std::make_unique<expressions::PrimitiveType>(typeName));

		return types.back().get();
	}

	auto *type = match->get();
	type->setDirty();

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::accept(plasp::pddl::ExpressionVisitor &expressionVisitor) const
{
	expressionVisitor.visit(*this);
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

const std::vector<const PrimitiveType *> &PrimitiveType::parentTypes() const
{
	return m_parentTypes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
