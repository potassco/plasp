#include <plasp/pddl/expressions/Predicate.h>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Domain.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/Problem.h>
#include <plasp/pddl/expressions/Constant.h>
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

	if (!parser.testAndSkip<std::string>("("))
	{
		parser.seek(position);
		return nullptr;
	}

	const auto predicateName = parser.parseIdentifier();
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

	auto predicate = PredicatePointer(new Predicate);

	predicate->m_name = predicateName;

	context.parser.skipWhiteSpace();

	// Parse arguments
	while (context.parser.currentCharacter() != ')')
	{
		// Parse variables
		if (context.parser.currentCharacter() == '?')
		{
			const auto variable = Variable::parseAndFind(context, expressionContext);
			predicate->m_arguments.emplace_back(variable);
		}
		// Parse constants
		else
		{
			const auto constant = (expressionContext.problem == nullptr)
				? Constant::parseAndFind(context, expressionContext.domain)
				: Constant::parseAndFind(context, *expressionContext.problem);
			predicate->m_arguments.emplace_back(constant);
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

	if (!parser.testAndSkip<std::string>("("))
	{
		parser.seek(position);
		return nullptr;
	}

	const auto predicateName = parser.parseIdentifier();
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

	auto predicate = PredicatePointer(new Predicate);

	predicate->m_name = predicateName;

	parser.skipWhiteSpace();

	while (parser.currentCharacter() != ')')
	{
		if (parser.currentCharacter() == '?')
			throw input::ParserException(parser.location(), "variables not allowed in this context");

		// Parse objects and constants
		const auto constant = Constant::parseAndFind(context, problem);
		predicate->m_arguments.emplace_back(constant);
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

void Predicate::print(std::ostream &ostream) const
{
	// TODO: implement correctly
	ostream << "(<predicate>)";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
