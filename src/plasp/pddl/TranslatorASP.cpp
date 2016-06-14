#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/utils/IO.h>
#include <plasp/utils/TranslatorException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(const Description &description, std::ostream &ostream)
:	m_description(description),
	m_ostream(ostream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	translateDomain();

	if (m_description.containsProblem())
	{
		m_ostream << std::endl;
		translateProblem();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateDomain() const
{
	m_ostream
		<< "%---------------------------------------" << std::endl
		<< "% domain" << std::endl
		<< "%---------------------------------------" << std::endl;

	const auto &domain = m_description.domain();

	// Types
	m_ostream << std::endl;
	translateTypes();

	// Constants
	if (!domain.constants().empty())
	{
		m_ostream << std::endl;
		translateConstants("constants", domain.constants());
	}

	// Predicates
	if (!domain.predicates().empty())
	{
		m_ostream << std::endl;
		translatePredicates();
	}

	// Actions
	if (!domain.actions().empty())
	{
		m_ostream << std::endl;
		translateActions();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateTypes() const
{
	m_ostream << "% types";

	const auto &types = m_description.domain().types();

	if (types.empty())
	{
		m_ostream << std::endl << "type(object)." << std::endl;
		return;
	}

	std::for_each(types.cbegin(), types.cend(),
		[&](const auto &type)
		{
			m_ostream << std::endl;

			const auto typeName = utils::escapeASP(type->name());

			m_ostream << "type(" << typeName << ")." << std::endl;

			const auto &parentTypes = type->parentTypes();

			std::for_each(parentTypes.cbegin(), parentTypes.cend(),
				[&](const auto &parentType)
				{
					const auto parentTypeName = utils::escapeASP(parentType->name());

					m_ostream << "inherits(type(" << typeName << "), type(" << parentTypeName << "))." << std::endl;
					m_ostream << "hasType(X, type(" << parentTypeName << ")) :- hasType(X, type(" << typeName << "))." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicates() const
{
	m_ostream << "% predicates";

	const auto &predicates = m_description.domain().predicates();

	std::for_each(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			m_ostream << std::endl;

			m_ostream << "predicate(" << utils::escapeASP(predicate->name());

			this->translateVariablesHead(predicate->arguments());

			m_ostream << ")";

			this->translateVariablesBody(predicate->arguments());

			m_ostream << ".";
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_ostream << "% actions";

	const auto &actions = m_description.domain().actions();

	const auto printActionName =
		[&](const auto &action)
		{
			m_ostream << "action(" << utils::escapeASP(action.name());

			this->translateVariablesHead(action.parameters());

			m_ostream << ")";
		};

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			const auto translateLiteral =
				[&](const auto &ruleHead, const auto &literal)
				{
					m_ostream << std::endl;
					m_ostream << ruleHead << "(";

					printActionName(*action);

					m_ostream << ", ";

					this->translateLiteral(literal);

					m_ostream << ") :- ";

					printActionName(*action);

					m_ostream << ".";
				};

			m_ostream << std::endl;

			// Name
			printActionName(*action);

			this->translateVariablesBody(action->parameters());

			m_ostream << ".";

			// Precondition
			if (action->precondition())
			{
				const auto &precondition = *action->precondition();

				if (precondition.expressionType() == Expression::Type::Predicate
					|| precondition.expressionType() == Expression::Type::Not)
				{
					translateLiteral("precondition", precondition);
				}
				// Assuming a conjunction
				else
				{
					if (precondition.expressionType() != Expression::Type::And)
						throw utils::TranslatorException("Only \"and\" expressions and (negated) predicates supported as action preconditions currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(precondition);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto *argument)
						{
							translateLiteral("precondition", *argument);
						});
				}
			}

			// Effect
			if (action->effect())
			{
				const auto &effect = *action->effect();

				if (effect.expressionType() == Expression::Type::Predicate
					|| effect.expressionType() == Expression::Type::Not)
				{
					translateLiteral("postcondition", effect);
				}
				// Assuming a conjunction
				else
				{
					if (effect.expressionType() != Expression::Type::And)
						throw utils::TranslatorException("Only \"and\" expressions and (negated) predicates supported as action effects currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(effect);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto *argument)
						{
							translateLiteral("postcondition", *argument);
						});
				}
			}

			m_ostream << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants(const std::string &header, const expressions::Constants &constants) const
{
	m_ostream << "% " << header;

	std::for_each(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			const auto constantName = utils::escapeASP(constant->name());

			m_ostream << std::endl << "constant(" << constantName << ")." << std::endl;

			const auto *type = constant->type();

			if (type != nullptr)
				m_ostream << "hasType(constant(" << constantName << "), type(" << utils::escapeASP(type->name()) << "))." << std::endl;
			else
				m_ostream << "hasType(constant(" << constantName << "), type(object))." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariablesHead(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	m_ostream << "(";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		if (i != variables.cbegin())
			m_ostream << ", ";

		const auto &variable = **i;

		m_ostream << utils::escapeASPVariable(variable.name());
	}

	m_ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariablesBody(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	m_ostream << " :- ";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		const auto &variable = **i;

		if (i != variables.cbegin())
			m_ostream << ", ";

		if (variable.type() != nullptr)
		{
			if (variable.type()->expressionType() != Expression::Type::PrimitiveType)
				throw utils::TranslatorException("Only primitive types supported currently");

			const auto &type = *dynamic_cast<const expressions::PrimitiveType *>(variable.type());

			m_ostream << "hasType(" << utils::escapeASPVariable(variable.name()) << ", type(" << type.name() << "))";
		}
		else
			m_ostream << "hasType(" << utils::escapeASPVariable(variable.name()) << ", type(object))";
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateLiteral(const Expression &literal) const
{
	// Translate single predicate
	if (literal.expressionType() == Expression::Type::Predicate)
	{
		this->translatePredicate(dynamic_cast<const expressions::Predicate &>(literal));
		m_ostream << ", true";
	}
	// Assuming that "not" expression may only contain a predicate
	else if (literal.expressionType() == Expression::Type::Not)
	{
		const auto &notExpression = dynamic_cast<const expressions::Not &>(literal);
		const auto &predicate = dynamic_cast<const expressions::Predicate &>(*notExpression.argument());

		this->translatePredicate(predicate);
		m_ostream << ", false";
	}
	else
		throw utils::TranslatorException("Only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicate(const expressions::Predicate &predicate) const
{
	m_ostream << "predicate(" << utils::escapeASP(predicate.name());

	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		m_ostream << ")";
		return;
	}

	m_ostream << "(";

	for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
	{
		if (i != arguments.cbegin())
			m_ostream << ", ";

		if ((*i)->expressionType() == Expression::Type::Constant)
		{
			const auto &constant = dynamic_cast<const expressions::Constant &>(**i);

			m_ostream << "constant(" << utils::escapeASP(constant.name()) << ")";
		}
		else if ((*i)->expressionType() == Expression::Type::Variable)
		{
			const auto &variable = dynamic_cast<const expressions::Variable &>(**i);

			m_ostream << utils::escapeASPVariable(variable.name());
		}
		else
			throw utils::TranslatorException("Only variables and constants supported in predicates currently");
	}

	m_ostream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_ostream
		<< "%---------------------------------------" << std::endl
		<< "% problem" << std::endl
		<< "%---------------------------------------" << std::endl;

	const auto &problem = m_description.problem();

	// Objects
	if (!problem.objects().empty())
	{
		m_ostream << std::endl;
		translateConstants("objects", problem.objects());
	}

	// Initial State
	m_ostream << std::endl;
	translateInitialState();

	// Goal
	m_ostream << std::endl;
	translateGoal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_ostream << "% initial state";

	const auto &initialStateFacts = m_description.problem().initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_ostream << std::endl << "initialState(";

			// Translate single predicate
			if (fact->expressionType() == Expression::Type::Predicate)
				this->translatePredicate(dynamic_cast<const expressions::Predicate &>(*fact));
			// Assuming that "not" expression may only contain a predicate
			else if (fact->expressionType() == Expression::Type::Not)
			{
				const auto &notExpression = dynamic_cast<const expressions::Not &>(*fact);

				if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
					throw utils::TranslatorException("Only negations of simple predicates supported in initial state currently");
			}
			else
				throw utils::TranslatorException("Only predicates and their negations supported in initial state currently");

			m_ostream << ").";
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_ostream << "% goal";

	const auto &goal = m_description.problem().goal();

	if (goal.expressionType() == Expression::Type::Predicate
		|| goal.expressionType() == Expression::Type::Not)
	{
		m_ostream << std::endl << "goal(";

		translateLiteral(goal);

		m_ostream << ").";
	}
	else if (goal.expressionType() == Expression::Type::And)
	{
		const auto &andExpression = dynamic_cast<const expressions::And &>(goal);

		std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
			[&](const auto *argument)
			{
				m_ostream << std::endl << "goal(";

				this->translateLiteral(*argument);

				m_ostream << ").";
			});
	}
	else
		throw utils::TranslatorException("Only single predicates, their negations, and conjunctions are currently supported in the goal");

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
