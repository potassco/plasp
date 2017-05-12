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
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("("))
	{
		tokenizer.seek(position);
		return nullptr;
	}

	const auto predicateName = tokenizer.getIdentifier();
	const auto &predicates = expressionContext.domain.predicates();

	const auto matchingPredicate = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	if (matchingPredicate == predicates.cend())
	{
		tokenizer.seek(position);
		return nullptr;
	}

	auto predicate = PredicatePointer(new Predicate);

	predicate->m_name = predicateName;

	tokenizer.skipWhiteSpace();

	// Parse arguments
	while (tokenizer.currentCharacter() != ')')
	{
		// Parse variables
		if (tokenizer.currentCharacter() == '?')
		{
			const auto variable = expressionContext.variables.parseAndFind(context);
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

		tokenizer.skipWhiteSpace();
	}

	// TODO: check that signature matches one of the declared ones

	tokenizer.expect<std::string>(")");

	return predicate;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

PredicatePointer Predicate::parse(Context &context, const Problem &problem)
{
	auto &tokenizer = context.tokenizer;

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("("))
	{
		tokenizer.seek(position);
		return nullptr;
	}

	const auto predicateName = tokenizer.getIdentifier();
	const auto &predicates = problem.domain().predicates();

	const auto matchingPredicate = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	if (matchingPredicate == predicates.cend())
	{
		tokenizer.seek(position);
		return nullptr;
	}

	auto predicate = PredicatePointer(new Predicate);

	predicate->m_name = predicateName;

	tokenizer.skipWhiteSpace();

	while (tokenizer.currentCharacter() != ')')
	{
		if (tokenizer.currentCharacter() == '?')
			throw tokenize::TokenizerException(tokenizer.location(), "variables not allowed in this context");

		// Parse objects and constants
		const auto constant = Constant::parseAndFind(context, problem);
		predicate->m_arguments.emplace_back(constant);
	}

	// TODO: check that signature matches one of the declared ones

	tokenizer.expect<std::string>(")");

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

ExpressionPointer Predicate::decomposed(DerivedPredicates &)
{
	// Predicates cannot be further decomposed
	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Predicate::collectParameters(std::set<VariablePointer> &parameters)
{
	for (const auto &argument : m_arguments)
		argument->collectParameters(parameters);
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
