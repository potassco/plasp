#include <plasp/pddl/expressions/Constant.h>

#include <algorithm>

#include <boost/assert.hpp>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/ExpressionVisitor.h>
#include <plasp/pddl/Problem.h>
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
	m_type{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ConstantPointer Constant::parseDeclaration(Context &context)
{
	context.parser.skipWhiteSpace();

	auto constant = std::make_unique<Constant>(Constant());

	constant->m_name = context.parser.parseIdentifier(isIdentifier);

	BOOST_ASSERT(constant->m_name != "-");

	// Flag constant for potentially upcoming type declaration
	constant->setDirty();

	// TODO: Store constant in hash map

	return constant;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::parseTypedDeclaration(Context &context, Domain &domain)
{
	parseTypedDeclaration(context, domain, domain.constants());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::parseTypedDeclaration(Context &context, Problem &problem)
{
	parseTypedDeclaration(context, problem.domain(), problem.objects());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::parseTypedDeclaration(Context &context, Domain &domain, Constants &constants)
{
	// Parse and store constant
	constants.emplace_back(parseDeclaration(context));

	context.parser.skipWhiteSpace();

	// Check for typing information
	if (!context.parser.probe('-'))
		return;

	// If existing, parse and store parent type
	auto *type = PrimitiveType::parseAndFind(context, domain);

	// Assign parent type to all types that were previously flagged
	std::for_each(constants.begin(), constants.end(),
		[&](auto &constant)
		{
			if (!constant->isDirty())
				return;

			constant->setType(type);
			constant->setDirty(false);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Constant::parseTypedDeclarations(Context &context, Domain &domain)
{
	while (context.parser.currentCharacter() != ')')
		parseTypedDeclaration(context, domain);

	if (domain.constants().empty())
		return;

	// Check correct use of typing requirement
	const auto typingUsed = (domain.constants().back()->type() != nullptr);
	const auto typingDeclared = domain.hasRequirement(Requirement::Type::Typing);

	if (!typingUsed && typingDeclared)
		throw utils::ParserException(context.parser, "Constant has undeclared type");

	if (typingUsed && !typingDeclared)
		throw utils::ParserException(context.parser, "Typing used but not declared as a requirement");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Constant *Constant::parseAndFind(Context &context, const ExpressionContext &expressionContext)
{
	context.parser.skipWhiteSpace();

	const auto constantName = context.parser.parseIdentifier(isIdentifier);

	auto *constant = parseAndFind(constantName, expressionContext.domain.constants());

	if (constant)
		return constant;

	if (expressionContext.problem)
	{
		constant = parseAndFind(constantName, expressionContext.problem->objects());

		if (constant)
			return constant;
	}

	throw utils::ParserException(context.parser, "Constant \"" + constantName + "\" used but never declared");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Constant *Constant::parseAndFind(const std::string &constantName, const Constants &constants)
{
	// TODO: use hash map
	const auto match = std::find_if(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			return constant->name() == constantName;
		});
	const auto constantExists = (match != constants.cend());

	if (!constantExists)
		return nullptr;

	return match->get();
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
