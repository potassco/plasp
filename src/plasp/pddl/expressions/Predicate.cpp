#include <plasp/pddl/expressions/Predicate.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/Identifier.h>
#include <plasp/pddl/Problem.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/Reference.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Predicate::Predicate()
:	m_isDeclared{false}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PredicatePointer Predicate::parse(Context &context, ExpressionContext &expressionContext)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("("))
	{
		parser.seek(position);
		return nullptr;
	}

	const auto predicateName = parser.parseIdentifier(isIdentifier);
	const auto &predicates = expressionContext.domain.predicates();

	const auto matchingPredicate = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	if (matchingPredicate == predicates.cend())
	{
		parser.seek(position);
		return nullptr;
	}

	auto predicate = std::make_unique<Predicate>(Predicate());

	predicate->m_name = predicateName;

	context.parser.skipWhiteSpace();

	// Parse arguments
	while (context.parser.currentCharacter() != ')')
	{
		// Parse variables
		if (context.parser.currentCharacter() == '?')
		{
			const auto *variable = Variable::parseAndFind(context, expressionContext);
			auto variableReference = std::make_unique<Reference<Variable>>(variable);
			predicate->m_arguments.emplace_back(std::move(variableReference));
		}
		// Parse constants
		else
		{
			const auto *constant = (expressionContext.problem == nullptr)
				? Constant::parseAndFind(context, expressionContext.domain)
				: Constant::parseAndFind(context, *expressionContext.problem);
			auto constantReference = std::make_unique<Reference<Constant>>(constant);
			predicate->m_arguments.emplace_back(std::move(constantReference));
		}

		context.parser.skipWhiteSpace();
	}

	// TODO: check that signature matches one of the declared ones

	parser.expect<std::string>(")");

	return predicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PredicatePointer Predicate::parse(Context &context, const Problem &problem)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.probe<std::string>("("))
	{
		parser.seek(position);
		return nullptr;
	}

	const auto predicateName = parser.parseIdentifier(isIdentifier);
	const auto &predicates = problem.domain().predicates();

	const auto matchingPredicate = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	if (matchingPredicate == predicates.cend())
	{
		parser.seek(position);
		return nullptr;
	}

	auto predicate = std::make_unique<Predicate>(Predicate());

	predicate->m_name = predicateName;

	context.parser.skipWhiteSpace();

	while (context.parser.currentCharacter() != ')')
	{
		if (context.parser.currentCharacter() == '?')
			throw utils::ParserException(context.parser, "Variables not allowed in this context");

		// Parse objects and constants
		const auto *constant = Constant::parseAndFind(context, problem);
		auto constantReference = std::make_unique<Reference<Constant>>(constant);
		predicate->m_arguments.emplace_back(std::move(constantReference));
	}

	// TODO: check that signature matches one of the declared ones

	parser.expect<std::string>(")");

	return predicate;
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

const Expressions &Predicate::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
