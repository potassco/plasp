#include <plasp/pddl/Predicate.h>

#include <algorithm>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Identifier.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate::Predicate(std::string name)
:	m_isDeclared{false},
	m_name{name}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate &Predicate::parseDeclaration(utils::Parser &parser, Context &context)
{
	parser.expect<std::string>("(");

	const auto predicateName = parser.parseIdentifier(isIdentifier);

	auto predicate = std::make_unique<Predicate>(Predicate(predicateName));

	// Flag predicate as correctly declared in the types section
	predicate->setDeclared();

	parser.skipWhiteSpace();

	// Parse arguments
	while (parser.currentCharacter() != ')')
	{
		predicate->m_arguments.emplace_back(Variable::parse(parser, context));

		parser.skipWhiteSpace();

		// Check if the variable has a type declaration
		if (!parser.advanceIf('-'))
			continue;

		auto parseType =
			[&]() -> TypePtr
			{
				parser.skipWhiteSpace();

				if (parser.currentCharacter() == '(')
					return TypePtr(&EitherType::parse(parser, context));

				return TypePtr(&PrimitiveType::parse(parser, context));
			};

		// Parse argument type
		const auto type = parseType();

		// Set the argument type for all previously flagged arguments
		std::for_each(predicate->m_arguments.begin(), predicate->m_arguments.end(),
			[&](auto &argument)
			{
				if (!argument.isDirty())
					return;

				argument.setType(type);
				argument.setDirty(false);
			});

		parser.skipWhiteSpace();
	}

	parser.expect<std::string>(")");

	const auto predicateArity = predicate->m_arguments.size();
	const PredicateHashMapKey key = {predicateName, predicateArity};

	// Store new predicate
	context.predicates.emplace_back(std::move(predicate));

	// Add a pointer to the predicate to the hash map
	context.predicatesHashMap.emplace(std::make_pair(key, context.predicates.back().get()));

	return *context.predicates.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::setDeclared()
{
	m_isDeclared = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Predicate::isDeclared() const
{
	return m_isDeclared;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string &Predicate::name() const
{
	return m_name;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Variables &Predicate::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
