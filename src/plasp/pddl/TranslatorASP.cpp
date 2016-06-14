#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/utils/Formatting.h>
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

TranslatorASP::TranslatorASP(const Description &description, utils::LogStream &outputStream)
:	m_description(description),
	m_outputStream(outputStream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	translateDomain();

	if (m_description.containsProblem())
	{
		m_outputStream << std::endl;
		translateProblem();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateDomain() const
{
	printHeading1("domain");

	const auto &domain = m_description.domain();

	// Types
	m_outputStream << std::endl;
	translateTypes();

	// Constants
	if (!domain.constants().empty())
	{
		m_outputStream << std::endl;
		translateConstants("constants", domain.constants());
	}

	// Predicates
	if (!domain.predicates().empty())
	{
		m_outputStream << std::endl;
		translatePredicates();
	}

	// Actions
	if (!domain.actions().empty())
	{
		m_outputStream << std::endl;
		translateActions();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateTypes() const
{
	printHeading2("types");

	m_outputStream << std::endl;

	const auto &types = m_description.domain().types();

	if (types.empty())
	{
		printKeyword("type");
		m_outputStream << "(object)." << std::endl;
		return;
	}

	std::for_each(types.cbegin(), types.cend(),
		[&](const auto &type)
		{
			const auto typeName = utils::escapeASP(type->name());

			printKeyword("type");
			m_outputStream << "(" << typeName << ")." << std::endl;

			const auto &parentTypes = type->parentTypes();

			std::for_each(parentTypes.cbegin(), parentTypes.cend(),
				[&](const auto &parentType)
				{
					const auto parentTypeName = utils::escapeASP(parentType->name());

					printKeyword("inherits");
					m_outputStream << "(";
					printKeyword("type");
					m_outputStream << "(" << typeName << "), ";
					printKeyword("type");
					m_outputStream << "(" << parentTypeName << "))." << std::endl;

					printKeyword("hasType");
					m_outputStream << "(";
					printVariable("X");
					m_outputStream << ", ";
					printKeyword("type");
					m_outputStream << "(" << parentTypeName << ")) :- ";
					printKeyword("hasType");
					m_outputStream << "(";
					printVariable("X");
					m_outputStream << ", ";
					printKeyword("type");
					m_outputStream << "(" << typeName << "))." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicates() const
{
	printHeading2("predicates");

	const auto &predicates = m_description.domain().predicates();

	std::for_each(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			m_outputStream << std::endl;

			printKeyword("predicate");
			m_outputStream << "(" << utils::escapeASP(predicate->name());

			this->translateVariablesHead(predicate->arguments());

			m_outputStream << ")";

			this->translateVariablesBody(predicate->arguments());

			m_outputStream << ".";
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	printHeading2("actions");

	const auto &actions = m_description.domain().actions();

	const auto printActionName =
		[&](const auto &action)
		{
			printKeyword("action");
			m_outputStream << "(" << utils::escapeASP(action.name());

			this->translateVariablesHead(action.parameters());

			m_outputStream << ")";
		};

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			const auto translateLiteral =
				[&](const auto &ruleHead, const auto &literal)
				{
					m_outputStream << std::endl;
					printKeyword(ruleHead);
					m_outputStream << "(";

					printActionName(*action);

					m_outputStream << ", ";

					this->translateLiteral(literal);

					m_outputStream << ") :- ";

					printActionName(*action);

					m_outputStream << ".";
				};

			m_outputStream << std::endl;

			// Name
			printActionName(*action);

			this->translateVariablesBody(action->parameters());

			m_outputStream << ".";

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
						throw utils::TranslatorException("Only “and” expressions and (negated) predicates supported as action preconditions currently");

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
						throw utils::TranslatorException("Only “and” expressions and (negated) predicates supported as action effects currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(effect);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto *argument)
						{
							translateLiteral("postcondition", *argument);
						});
				}
			}

			m_outputStream << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants(const std::string &heading, const expressions::Constants &constants) const
{
	printHeading2(heading);

	std::for_each(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			const auto constantName = utils::escapeASP(constant->name());

			m_outputStream << std::endl;
			printKeyword("constant");
			m_outputStream << "(" << constantName << ")." << std::endl;

			const auto *type = constant->type();

			if (type != nullptr)
			{
				printKeyword("hasType");
				m_outputStream << "(";
				printKeyword("constant");
				m_outputStream << "(" << constantName << "), ";
				printKeyword("type");
				m_outputStream << "(" << utils::escapeASP(type->name()) << "))." << std::endl;
			}
			else
			{
				printKeyword("hasType");
				m_outputStream << "(";
				printKeyword("constant");
				m_outputStream << "(" << constantName << "), ";
				printKeyword("type");
				m_outputStream << "(object))." << std::endl;
			}
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariablesHead(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	m_outputStream << "(";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		if (i != variables.cbegin())
			m_outputStream << ", ";

		const auto &variable = **i;

		printVariable(utils::escapeASPVariable(variable.name()));
	}

	m_outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariablesBody(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	m_outputStream << " :- ";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		const auto &variable = **i;

		if (i != variables.cbegin())
			m_outputStream << ", ";

		if (variable.type() != nullptr)
		{
			if (variable.type()->expressionType() != Expression::Type::PrimitiveType)
				throw utils::TranslatorException("Only primitive types supported currently");

			const auto &type = *dynamic_cast<const expressions::PrimitiveType *>(variable.type());

			printKeyword("hasType");
			m_outputStream << "(";
			printVariable(utils::escapeASPVariable(variable.name()));
			m_outputStream << ", ";
			printKeyword("type");
			m_outputStream << "(" << type.name() << "))";
		}
		else
		{
			printKeyword("hasType");
			m_outputStream << "(";
			printVariable(utils::escapeASPVariable(variable.name()));
			m_outputStream << ", ";
			printKeyword("type");
			m_outputStream << "(object))";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateLiteral(const Expression &literal) const
{
	// Translate single predicate
	if (literal.expressionType() == Expression::Type::Predicate)
	{
		this->translatePredicate(dynamic_cast<const expressions::Predicate &>(literal));
		m_outputStream << ", ";
		printKeyword("true");
	}
	// Assuming that "not" expression may only contain a predicate
	else if (literal.expressionType() == Expression::Type::Not)
	{
		const auto &notExpression = dynamic_cast<const expressions::Not &>(literal);
		const auto &predicate = dynamic_cast<const expressions::Predicate &>(*notExpression.argument());

		this->translatePredicate(predicate);
		m_outputStream << ", ";
		printKeyword("false");
	}
	else
		throw utils::TranslatorException("Only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicate(const expressions::Predicate &predicate) const
{
	printKeyword("predicate");
	m_outputStream << "(" << utils::escapeASP(predicate.name());

	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		m_outputStream << ")";
		return;
	}

	m_outputStream << "(";

	for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
	{
		if (i != arguments.cbegin())
			m_outputStream << ", ";

		if ((*i)->expressionType() == Expression::Type::Constant)
		{
			const auto &constant = dynamic_cast<const expressions::Constant &>(**i);

			printKeyword("constant");
			m_outputStream << "(" << utils::escapeASP(constant.name()) << ")";
		}
		else if ((*i)->expressionType() == Expression::Type::Variable)
		{
			const auto &variable = dynamic_cast<const expressions::Variable &>(**i);

			printVariable(utils::escapeASPVariable(variable.name()));
		}
		else
			throw utils::TranslatorException("Only variables and constants supported in predicates currently");
	}

	m_outputStream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	BOOST_ASSERT(m_description.containsProblem());

	printHeading1("problem");

	const auto &problem = m_description.problem();

	// Objects
	if (!problem.objects().empty())
	{
		m_outputStream << std::endl;
		translateConstants("objects", problem.objects());
	}

	// Initial State
	m_outputStream << std::endl;
	translateInitialState();

	// Goal
	m_outputStream << std::endl;
	translateGoal();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateInitialState() const
{
	BOOST_ASSERT(m_description.containsProblem());

	printHeading2("initial state");

	const auto &initialStateFacts = m_description.problem().initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << std::endl;
			printKeyword("initialState");
			m_outputStream << "(";

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

			m_outputStream << ").";
		});

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	BOOST_ASSERT(m_description.containsProblem());

	printHeading2("goal");

	const auto &goal = m_description.problem().goal();

	if (goal.expressionType() == Expression::Type::Predicate
		|| goal.expressionType() == Expression::Type::Not)
	{
		m_outputStream << std::endl;
		printKeyword("goal");
		m_outputStream << "(";

		translateLiteral(goal);

		m_outputStream << ").";
	}
	else if (goal.expressionType() == Expression::Type::And)
	{
		const auto &andExpression = dynamic_cast<const expressions::And &>(goal);

		std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
			[&](const auto *argument)
			{
				m_outputStream << std::endl;
				printKeyword("goal");
				m_outputStream << "(";

				this->translateLiteral(*argument);

				m_outputStream << ").";
			});
	}
	else
		throw utils::TranslatorException("Only single predicates, their negations, and conjunctions are currently supported in the goal");

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::printHeading1(const std::string &heading) const
{
	m_outputStream
		<< utils::Format(utils::Color::Blue, utils::FontWeight::Bold)
		<< "%---------------------------------------" << std::endl
		<< "% " << heading << std::endl
		<< "%---------------------------------------"
		<< utils::ResetFormat()
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::printHeading2(const std::string &heading) const
{
	m_outputStream
		<< utils::Format(utils::Color::Blue, utils::FontWeight::Bold)
		<< "% " << heading
		<< utils::ResetFormat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::printKeyword(const std::string &keyword) const
{
	m_outputStream
		<< utils::Format(utils::Color::White, utils::FontWeight::Bold)
		<< keyword
		<< utils::ResetFormat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::printVariable(const std::string &variable) const
{
	m_outputStream
		<< utils::Format(utils::Color::Green, utils::FontWeight::Bold)
		<< variable
		<< utils::ResetFormat();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
