#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <colorlog/Formatting.h>

#include <pddlparse/AST.h>

#include <plasp/TranslatorException.h>

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

TranslatorASP::TranslatorASP(const ::pddl::ast::Description &description, colorlog::ColorStream &outputStream)
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
	m_outputStream << colorlog::Heading1("domain");

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
	m_outputStream << colorlog::Heading2("types");

	m_outputStream << std::endl;

	const auto &types = m_description.domain->types;

	if (types.empty())
	{
		m_outputStream
			<< colorlog::Function("type") << "("
			<< colorlog::Keyword("type") << "(" << colorlog::String("object") << "))." << std::endl;

		return;
	}

	for (const auto &type : types)
	{
		m_outputStream
			<< colorlog::Function("type") << "("
			<< colorlog::Keyword("type") << "("
			<< *type
			<< "))." << std::endl;

		const auto &parentTypes = type->parentTypes;

		std::for_each(parentTypes.cbegin(), parentTypes.cend(),
			[&](const auto &parentType)
			{
				m_outputStream
					<< colorlog::Function("inherits") << "(" << colorlog::Keyword("type")
					<< "(" << *type << "), " << colorlog::Keyword("type")
					<< "(" << *parentType << "))." << std::endl;
			});
	}

	m_outputStream
		<< std::endl
		<< colorlog::Function("has") << "("
		<< colorlog::Variable("X") << ", "
		<< colorlog::Keyword("type") << "(" << colorlog::Variable("T2") << ")) :- "
		<< colorlog::Function("has") << "("
		<< colorlog::Variable("X") << ", "
		<< colorlog::Keyword("type") << "(" << colorlog::Variable("T1") << ")), "
		<< colorlog::Function("inherits") << "("
		<< colorlog::Keyword("type") << "(" << colorlog::Variable("T1") << "), "
		<< colorlog::Keyword("type") << "(" << colorlog::Variable("T2") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicates() const
{
	m_outputStream << colorlog::Heading2("variables");

	const auto &predicates = m_description.domain->predicates;

	for (const auto &predicate : predicates)
	{
		m_outputStream << std::endl << colorlog::Function("variable") << "(";

		translatePredicateDeclaration(m_outputStream, *predicate);

		m_outputStream << ")";

		translateVariablesForRuleBody(m_outputStream, predicate->parameters);

		m_outputStream << ".";
	}

	m_outputStream
		<< std::endl << std::endl
		<< colorlog::Function("boolean") << "(" << colorlog::Boolean("true") << ")." << std::endl
		<< colorlog::Function("boolean") << "(" << colorlog::Boolean("false") << ")." << std::endl
		<< std::endl
		<< colorlog::Function("contains") << "("
		<< colorlog::Keyword("variable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Variable("B") << ")) :- "
		<< colorlog::Function("variable") << "(" << colorlog::Keyword("variable") << "(" << colorlog::Variable("X") << ")), "
		<< colorlog::Function("boolean") << "(" << colorlog::Variable("B") << ")."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << colorlog::Heading2("actions");

	const auto &actions = m_description.domain->actions;

	for (const auto &action : actions)
	{
		const auto printActionName =
			[&]()
			{
				m_outputStream << colorlog::Keyword("action") << "(";

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
		m_outputStream << colorlog::Function("action") << "(";
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
	m_outputStream << colorlog::Heading2(heading.c_str());

	for (const auto &constant : constants)
	{
		m_outputStream << std::endl
			<< colorlog::Function("constant") << "("
			<< colorlog::Keyword("constant") << "("
			<< *constant
			<< "))." << std::endl;

		const auto &type = constant->type;

		if (type)
		{
			if (!type.value().is<::pddl::ast::PrimitiveTypePointer>())
				throw TranslatorException("only primitive types supported currently");

			const auto &primitveType = type.value().get<::pddl::ast::PrimitiveTypePointer>();

			m_outputStream << colorlog::Function("has") << "("
				<< colorlog::Keyword("constant") << "(" << *constant << "), "
				<< colorlog::Keyword("type") << "(" << *primitveType << "))." << std::endl;
		}
		else
		{
			m_outputStream << colorlog::Function("has") << "("
				<< colorlog::Keyword("constant") << "(" << *constant << "), "
				<< colorlog::Keyword("type") << "(" << colorlog::String("object") << "))." << std::endl;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	assert(m_description.problem);

	m_outputStream << colorlog::Heading1("problem");

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

	m_outputStream << colorlog::Heading2("initial state");

	const auto &facts = m_description.problem.value()->initialState.facts;

	// TODO: move to separate header
	for (const auto &fact : facts)
	{
		m_outputStream << std::endl << colorlog::Function("initialState") << "(";

		const auto handleUnsupported =
			[&](const auto &)
			{
				throw TranslatorException("only predicates and their negations supported in initial state currently");
			};

		const auto handleAtomicFormula =
			[&](const ::pddl::ast::AtomicFormula &atomicFormula, bool isPositive = true)
			{
				if (!atomicFormula.is<::pddl::ast::PredicatePointer>())
					handleUnsupported(atomicFormula);

				const auto &predicate = atomicFormula.get<::pddl::ast::PredicatePointer>();

				translatePredicateToVariable(m_outputStream, *predicate, isPositive);
			};

		const auto handleNot =
			[&](const ::pddl::ast::NotPointer<::pddl::ast::AtomicFormula> &not_)
			{
				handleAtomicFormula(not_->argument, false);
			};

		const auto handleLiteral =
			[&](const ::pddl::ast::Literal &literal)
			{
				literal.match(handleAtomicFormula, handleNot);
			};

		fact.match(handleLiteral, handleUnsupported);

		m_outputStream << ").";
	}

	m_outputStream
		<< std::endl << std::endl
		<< colorlog::Function("initialState") << "("
		<< colorlog::Keyword("variable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Boolean("false") << ")) :- "
		<< colorlog::Function("variable") << "(" << colorlog::Keyword("variable") << "(" << colorlog::Variable("X") << ")), "
		<< colorlog::Keyword("not") << " "
		<< colorlog::Function("initialState") << "("
		<< colorlog::Keyword("variable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Boolean("true") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	assert(m_description.problem);
	assert(m_description.problem.value()->goal);

	m_outputStream << colorlog::Heading2("goal");

	const auto &goal = m_description.problem.value()->goal.value();
	// TODO: refactor
	::plasp::pddl::translateGoal(m_outputStream, goal);

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
