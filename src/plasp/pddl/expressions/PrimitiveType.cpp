#include <plasp/pddl/expressions/PrimitiveType.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
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
:	m_isDirty{false},
	m_isDeclared{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PrimitiveTypePointer PrimitiveType::parseDeclaration(utils::Parser &parser, Context &context)
{
	// TODO: refactor
	if (context.primitiveTypes.empty())
	{
		auto object = std::make_unique<PrimitiveType>(PrimitiveType());
		object->m_name = "object";
		object->setDirty();
		object->setDeclared();

		context.primitiveTypes.emplace_back(std::move(object));
	}

	parser.skipWhiteSpace();

	auto type = std::make_unique<PrimitiveType>(PrimitiveType());

	type->m_name = parser.parseIdentifier(isIdentifier);

	// Flag type for potentially upcoming parent type declaration
	type->setDirty();

	// TODO: Store constant in hash map

	return type;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void PrimitiveType::parseTypedDeclaration(utils::Parser &parser, Context &context)
{
	// Parse and store type
	context.primitiveTypes.emplace_back(parseDeclaration(parser, context));

	const auto &type = context.primitiveTypes.back();

	// Flag type as correctly declared in the types section
	type->setDeclared();

	parser.skipWhiteSpace();

	// Check for type inheritance
	if (!parser.advanceIf('-'))
		return;

	// If existing, parse and store parent type
	auto *parentType = parseExisting(parser, context.primitiveTypes);

	parentType->setDirty(false);

	// Flag parent tpe as correctly declared in the types section
	parentType->setDeclared();

	// Assign parent type to all types that were previously flagged
	std::for_each(context.primitiveTypes.begin(), context.primitiveTypes.end(),
		[&](auto &childType)
		{
			if (!childType->isDirty())
				return;

			childType->addParentType(parentType);
			childType->setDirty(false);
		});
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

void PrimitiveType::addParentType(const PrimitiveType *parentType)
{
	m_parentTypes.push_back(parentType);
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
