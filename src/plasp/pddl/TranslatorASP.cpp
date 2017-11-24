#include <plasp/pddl/TranslatorASP.h>

#include <cassert>

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>

#include <plasp/TranslatorException.h>

#include <plasp/pddl/translation/DerivedPredicate.h>
#include <plasp/pddl/translation/DerivedPredicatePrecondition.h>
#include <plasp/pddl/translation/Effect.h>
#include <plasp/pddl/translation/Fact.h>
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

TranslatorASP::TranslatorASP(const ::pddl::normalizedAST::Description &description, colorlog::ColorStream &outputStream)
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

	// Utils
	m_outputStream << std::endl;
	translateUtils();

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

	// Derived predicates
	if (!domain->derivedPredicates.empty())
	{
		m_outputStream << std::endl;
		translateDerivedPredicates(domain->derivedPredicates);
	}

	// Actions
	if (!domain->actions.empty())
	{
		m_outputStream << std::endl;
		translateActions();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateUtils() const
{
	m_outputStream << colorlog::Heading2("utils");

	m_outputStream
		<< std::endl
		<< colorlog::Function("boolean") << "(" << colorlog::Boolean("true") << ")." << std::endl
		<< colorlog::Function("boolean") << "(" << colorlog::Boolean("false") << ")." << std::endl;
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
		VariableIDMap variableIDs;

		m_outputStream << std::endl << colorlog::Function("variable") << "(";

		translatePredicateDeclaration(m_outputStream, *predicate, variableIDs);

		m_outputStream << ")";

		if (!predicate->parameters.empty())
		{
			m_outputStream << " :- ";

			VariableIDMap variableIDs;

			translateVariablesForRuleBody(m_outputStream, predicate->parameters, variableIDs);
		}

		m_outputStream << ".";
	}

	m_outputStream
		<< std::endl << std::endl
		<< colorlog::Function("contains") << "("
		<< colorlog::Variable("X") << ", "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Variable("B") << ")) :- "
		<< colorlog::Function("variable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Function("boolean") << "(" << colorlog::Variable("B") << ")."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateDerivedPredicates(const ::pddl::normalizedAST::DerivedPredicateDeclarations &derivedPredicates) const
{
	m_outputStream << colorlog::Heading2("derived predicates");

	for (const auto &derivedPredicate : derivedPredicates)
	{
		VariableIDMap variableIDs;

		m_outputStream << std::endl << colorlog::Function("derivedVariable") << "(";

		translateDerivedPredicateDeclaration(m_outputStream, *derivedPredicate, variableIDs);

		m_outputStream << ")";

		if (!derivedPredicate->parameters.empty())
			m_outputStream << " :- ";

		translateVariablesForRuleBody(m_outputStream, derivedPredicate->parameters, variableIDs);

		m_outputStream << ".";
	}

	m_outputStream
		<< std::endl << std::endl
		<< colorlog::Function("contains") << "("
		<< colorlog::Variable("X") << ", "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Variable("B") << ")) :- "
		<< colorlog::Function("derivedVariable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Function("boolean") << "(" << colorlog::Variable("B") << ")."
		<< std::endl;

	for (const auto &derivedPredicate : derivedPredicates)
	{
		VariableIDMap variableIDs;

		const auto printDerivedPredicateName =
			[&]()
			{
				m_outputStream << colorlog::Keyword("derivedPredicate") << "(";

				if (derivedPredicate->parameters.empty() && derivedPredicate->existentialParameters.empty())
				{
					m_outputStream << *derivedPredicate << ")";
					return;
				}

				m_outputStream << "(" << *derivedPredicate;

				translateVariablesForRuleHead(m_outputStream, derivedPredicate->parameters, variableIDs);
				translateVariablesForRuleHead(m_outputStream, derivedPredicate->existentialParameters, variableIDs);

				m_outputStream << ")), " << colorlog::Keyword("type") << "(";

				if (derivedPredicate->precondition.value().is<::pddl::normalizedAST::OrPointer<::pddl::normalizedAST::Literal>>())
					m_outputStream << colorlog::Reserved("or");
				else
					m_outputStream << colorlog::Reserved("and");

				m_outputStream << ")";
			};

		m_outputStream << std::endl;

		// Name
		m_outputStream << colorlog::Function("derivedPredicate") << "(";
		printDerivedPredicateName();
		m_outputStream << ")";

		if (!derivedPredicate->parameters.empty() || !derivedPredicate->existentialParameters.empty())
			m_outputStream << " :- ";

		translateVariablesForRuleBody(m_outputStream, derivedPredicate->parameters, variableIDs);

		if (!derivedPredicate->existentialParameters.empty() && !derivedPredicate->parameters.empty())
			m_outputStream << ", ";

		translateVariablesForRuleBody(m_outputStream, derivedPredicate->existentialParameters, variableIDs);

		m_outputStream << ".";

		// Precondition
		if (derivedPredicate->precondition)
			translateDerivedPredicatePrecondition(m_outputStream, derivedPredicate->precondition.value(), "derivedPredicate", printDerivedPredicateName, variableIDs);

		m_outputStream << std::endl << colorlog::Function("postcondition") << "(";
		printDerivedPredicateName();
		m_outputStream
			<< ", " << colorlog::Keyword("effect") << "("
			<< colorlog::Reserved("unconditional") << ")"
			<< ", ";
		translateDerivedPredicateDeclarationToVariable(m_outputStream, *derivedPredicate, variableIDs, true);
		m_outputStream << ") :- " << colorlog::Function("derivedPredicate") << "(";
		printDerivedPredicateName();
		m_outputStream << ").";

		m_outputStream << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << colorlog::Heading2("actions");

	const auto &actions = m_description.domain->actions;

	size_t numberOfConditionalEffects{0};

	for (const auto &action : actions)
	{
		VariableIDMap variableIDs;

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
				translateVariablesForRuleHead(m_outputStream, action->parameters, variableIDs);
				m_outputStream << "))";
			};

		const auto printPreconditionRuleBody =
			[&]()
			{
				m_outputStream << " :- " << colorlog::Function("action") << "(";
				printActionName();
				m_outputStream << ")";
			};

		m_outputStream << std::endl;

		// Name
		m_outputStream << colorlog::Function("action") << "(";
		printActionName();
		m_outputStream << ")";

		if (!action->parameters.empty())
		{
			m_outputStream << " :- ";
			translateVariablesForRuleBody(m_outputStream, action->parameters, variableIDs);
		}

		m_outputStream << ".";

		// Precondition
		if (action->precondition)
			translatePrecondition(m_outputStream, action->precondition.value(), printActionName,
				printPreconditionRuleBody, variableIDs);

		// Effect
		if (action->effect)
			translateEffect(m_outputStream, action->effect.value(), printActionName,
				numberOfConditionalEffects, variableIDs);

		m_outputStream << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants(const std::string &heading, const ::pddl::normalizedAST::ConstantDeclarations &constants) const
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
			if (!type.value().is<::pddl::normalizedAST::PrimitiveTypePointer>())
				throw TranslatorException("only primitive types supported currently");

			const auto &primitveType = type.value().get<::pddl::normalizedAST::PrimitiveTypePointer>();

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

	// Initial state
	m_outputStream << std::endl;
	translateInitialState();

	// Derived predicates
	if (!problem->derivedPredicates.empty())
	{
		m_outputStream << std::endl;
		translateDerivedPredicates(problem->derivedPredicates);
	}

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

	for (const auto &fact : facts)
		::plasp::pddl::translateFact(m_outputStream, fact);

	m_outputStream
		<< std::endl << std::endl
		<< colorlog::Function("initialState") << "("
		<< colorlog::Variable("X") << ", "
		<< colorlog::Keyword("value") << "(" << colorlog::Variable("X") << ", " << colorlog::Boolean("false") << ")) :- "
		<< colorlog::Function("variable") << "(" << colorlog::Variable("X") << "), "
		<< colorlog::Keyword("not") << " "
		<< colorlog::Function("initialState") << "("
		<< colorlog::Variable("X") << ", "
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

	::plasp::pddl::translateGoal(m_outputStream, goal);

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
