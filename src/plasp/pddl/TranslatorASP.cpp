#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <pddlparse/AST.h>

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>

#include <plasp/pddl/translation/Effect.h>
#include <plasp/pddl/translation/Goal.h>
#include <plasp/pddl/translation/Precondition.h>
#include <plasp/pddl/translation/Predicate.h>
#include <plasp/pddl/translation/Primitives.h>
#include <plasp/pddl/translation/Variables.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const ::pddl::ast::Description &description, output::ColorStream &outputStream)
:	m_description{description},
	m_outputStream(outputStream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	translateDomain();

	if (m_description.problem)
	{
		m_outputStream << std::endl;
		translateProblem();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateDomain() const
{
	m_outputStream << output::Heading1("domain");

	const auto &domain = m_description.domain;

	// Types
	m_outputStream << std::endl;
	translateTypes();

	// Constants
	if (!domain->constants.empty())
	{
		m_outputStream << std::endl;
		translateConstants("constants", domain->constants);
	}

	// Predicates
	if (!domain->predicates.empty())
	{
		m_outputStream << std::endl;
		translatePredicates();
	}

	// Actions
	if (!domain->actions.empty())
	{
		m_outputStream << std::endl;
		translateActions();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateTypes() const
{
	m_outputStream << output::Heading2("types");

	m_outputStream << std::endl;

	const auto &types = m_description.domain->types;

	if (types.empty())
	{
		m_outputStream
			<< output::Function("type") << "("
			<< output::Keyword("type") << "(" << output::String("object") << "))." << std::endl;

		return;
	}

	for (const auto &type : types)
	{
		m_outputStream
			<< output::Function("type") << "("
			<< output::Keyword("type") << "("
			<< *type
			<< "))." << std::endl;

		const auto &parentTypes = type->parentTypes;

		std::for_each(parentTypes.cbegin(), parentTypes.cend(),
			[&](const auto &parentType)
			{
				m_outputStream
					<< output::Function("inherits") << "(" << output::Keyword("type")
					<< "(" << *type << "), " << output::Keyword("type")
					<< "(" << *parentType << "))." << std::endl;
			});
	}

	m_outputStream
		<< std::endl
		<< output::Function("has") << "("
		<< output::Variable("X") << ", "
		<< output::Keyword("type") << "(" << output::Variable("T2") << ")) :- "
		<< output::Function("has") << "("
		<< output::Variable("X") << ", "
		<< output::Keyword("type") << "(" << output::Variable("T1") << ")), "
		<< output::Function("inherits") << "("
		<< output::Keyword("type") << "(" << output::Variable("T1") << "), "
		<< output::Keyword("type") << "(" << output::Variable("T2") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicates() const
{
	m_outputStream << output::Heading2("variables");

	const auto &predicates = m_description.domain->predicates;

	for (const auto &predicate : predicates)
	{
		m_outputStream << std::endl << output::Function("variable") << "(";

		translatePredicateDeclaration(m_outputStream, *predicate);

		m_outputStream << ")";

		translateVariablesForRuleBody(m_outputStream, predicate->parameters);

		m_outputStream << ".";
	}

	m_outputStream
		<< std::endl << std::endl
		<< output::Function("boolean") << "(" << output::Boolean("true") << ")." << std::endl
		<< output::Function("boolean") << "(" << output::Boolean("false") << ")." << std::endl
		<< std::endl
		<< output::Function("contains") << "("
		<< output::Keyword("variable") << "(" << output::Variable("X") << "), "
		<< output::Keyword("value") << "(" << output::Variable("X") << ", " << output::Variable("B") << ")) :- "
		<< output::Function("variable") << "(" << output::Keyword("variable") << "(" << output::Variable("X") << ")), "
		<< output::Function("boolean") << "(" << output::Variable("B") << ")."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << output::Heading2("actions");

	const auto &actions = m_description.domain->actions;

	for (const auto &action : actions)
	{
		const auto printActionName =
			[&]()
			{
				m_outputStream << output::Keyword("action") << "(";

				if (action->parameters.empty())
				{
					m_outputStream << *action << ")";
					return;
				}

				m_outputStream << "(" << *action;
				translateVariablesForRuleHead(m_outputStream, action->parameters);
				m_outputStream << "))";
			};

		m_outputStream << std::endl;

		// Name
		m_outputStream << output::Function("action") << "(";
		printActionName();
		m_outputStream << ")";

		translateVariablesForRuleBody(m_outputStream, action->parameters);

		m_outputStream << ".";

		// Precondition
		if (action->precondition)
			translatePrecondition(m_outputStream, action->precondition.value(), "action", printActionName);

		if (action->effect)
			translateEffect(m_outputStream, action->effect.value(), "action", printActionName);

		m_outputStream << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants(const std::string &heading, const ::pddl::ast::ConstantDeclarations &constants) const
{
	m_outputStream << output::Heading2(heading.c_str());

	for (const auto &constant : constants)
	{
		m_outputStream << std::endl
			<< output::Function("constant") << "("
			<< output::Keyword("constant") << "("
			<< *constant
			<< "))." << std::endl;

		const auto &type = constant->type;

		if (type)
		{
			if (!type.value().is<::pddl::ast::PrimitiveTypePointer>())
				throw output::TranslatorException("only primitive types supported currently");

			const auto &primitveType = type.value().get<::pddl::ast::PrimitiveTypePointer>();

			m_outputStream << output::Function("has") << "("
				<< output::Keyword("constant") << "(" << *constant << "), "
				<< output::Keyword("type") << "(" << *primitveType << "))." << std::endl;
		}
		else
		{
			m_outputStream << output::Function("has") << "("
				<< output::Keyword("constant") << "(" << *constant << "), "
				<< output::Keyword("type") << "(" << output::String("object") << "))." << std::endl;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	assert(m_description.problem);

	m_outputStream << output::Heading1("problem");

	const auto &problem = m_description.problem.value();

	// Objects
	if (!problem->objects.empty())
	{
		m_outputStream << std::endl;
		translateConstants("objects", problem->objects);
	}

	// Initial State
	m_outputStream << std::endl;
	translateInitialState();

	// Goal
	if (problem->goal)
	{
		m_outputStream << std::endl;
		translateGoal();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	assert(m_description.problem);

	m_outputStream << output::Heading2("initial state");

	const auto &facts = m_description.problem.value()->initialState.facts;

	for (const auto &fact : facts)
	{
		m_outputStream << std::endl << output::Function("initialState") << "(";

		// Translate single predicate
		if (fact.is<::pddl::ast::AtomicFormula>() && fact.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
		{
			const auto &predicate = fact.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			translatePredicateToVariable(m_outputStream, *predicate, true);
		}
		// Assuming that "not" expression may only contain a predicate
		else if (fact.is<::pddl::ast::NotPointer<::pddl::ast::Fact>>())
		{
			const auto &notExpression = fact.get<::pddl::ast::NotPointer<::pddl::ast::Fact>>();

			if (!notExpression->argument.is<::pddl::ast::AtomicFormula>() || !notExpression->argument.get<::pddl::ast::AtomicFormula>().is<::pddl::ast::PredicatePointer>())
				throw output::TranslatorException("only negations of simple predicates supported in initial state currently");

			const auto &predicate = notExpression->argument.get<::pddl::ast::AtomicFormula>().get<::pddl::ast::PredicatePointer>();

			translatePredicateToVariable(m_outputStream, *predicate, false);
		}
		else
			throw output::TranslatorException("only predicates and their negations supported in initial state currently");

		m_outputStream << ").";
	}

	m_outputStream
		<< std::endl << std::endl
		<< output::Function("initialState") << "("
		<< output::Keyword("variable") << "(" << output::Variable("X") << "), "
		<< output::Keyword("value") << "(" << output::Variable("X") << ", " << output::Boolean("false") << ")) :- "
		<< output::Function("variable") << "(" << output::Keyword("variable") << "(" << output::Variable("X") << ")), "
		<< output::Keyword("not") << " "
		<< output::Function("initialState") << "("
		<< output::Keyword("variable") << "(" << output::Variable("X") << "), "
		<< output::Keyword("value") << "(" << output::Variable("X") << ", " << output::Boolean("true") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	assert(m_description.problem);
	assert(m_description.problem.value()->goal);

	m_outputStream << output::Heading2("goal");

	const auto &goal = m_description.problem.value()->goal.value();
	// TODO: refactor
	::plasp::pddl::translateGoal(m_outputStream, goal);

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
