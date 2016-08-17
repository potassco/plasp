#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/utils/Formatting.h>
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
	m_outputStream << utils::Heading1("domain");

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
	m_outputStream << utils::Heading2("types");

	m_outputStream << std::endl;

	const auto &types = m_description.domain().types();

	if (types.empty())
	{
		m_outputStream
			<< utils::RuleName("type") << "("
			<< utils::Keyword("type") << "(" << utils::String("object") << ")." << std::endl;

		return;
	}

	std::for_each(types.cbegin(), types.cend(),
		[&](const auto &type)
		{
			m_outputStream
				<< utils::RuleName("type") << "("
				<< utils::Keyword("type") << "("
				<< utils::String(type->name())
				<< "))." << std::endl;

			const auto &parentTypes = type->parentTypes();

			std::for_each(parentTypes.cbegin(), parentTypes.cend(),
				[&](const auto &parentType)
				{
					m_outputStream
						<< utils::RuleName("inherits") << "(" << utils::Keyword("type")
						<< "(" << utils::String(type->name()) << "), " << utils::Keyword("type")
						<< "(" << utils::String(parentType->name()) << "))." << std::endl;
				});
		});

	m_outputStream
		<< std::endl
		<< utils::RuleName("has") << "("
		<< utils::Variable("X") << ", "
		<< utils::Keyword("type") << "(" << utils::Variable("T2") << ")) :- "
		<< utils::RuleName("has") << "("
		<< utils::Variable("X") << ", "
		<< utils::Keyword("type") << "(" << utils::Variable("T1") << ")), "
		<< utils::RuleName("inherits") << "("
		<< utils::Keyword("type") << "(" << utils::Variable("T1") << "), "
		<< utils::Keyword("type") << "(" << utils::Variable("T2") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicates() const
{
	m_outputStream << utils::Heading2("variables");

	const auto &predicates = m_description.domain().predicates();

	const auto printPredicateName =
		[&](const auto &predicate)
		{
			if (predicate->arguments().empty())
			{
				m_outputStream << utils::String(predicate->name());

				return;
			}

			m_outputStream << "(" << utils::String(predicate->name());
			this->translateVariablesHead(predicate->arguments());
			m_outputStream << ")";
		};

	std::for_each(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			m_outputStream
				<< std::endl
				<< utils::RuleName("variable") << "("
				<< utils::Keyword("variable") << "(";

			printPredicateName(predicate);

			m_outputStream << "))";

			this->translateVariablesBody(predicate->arguments());

			m_outputStream << ".";
		});

	m_outputStream
		<< std::endl << std::endl
		<< utils::RuleName("boolean") << "(" << utils::Boolean("true") << ")." << std::endl
		<< utils::RuleName("boolean") << "(" << utils::Boolean("false") << ")." << std::endl
		<< std::endl
		<< utils::RuleName("contains") << "("
		<< utils::Keyword("variable") << "(" << utils::Variable("X") << "), "
		<< utils::Keyword("value") << "(" << utils::Variable("X") << ", " << utils::Variable("B") << ")) :- "
		<< utils::RuleName("variable") << "(" << utils::Keyword("variable") << "(" << utils::Variable("X") << ")), "
		<< utils::RuleName("boolean") << "(" << utils::Variable("B") << ")."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_outputStream << utils::Heading2("actions");

	const auto &actions = m_description.domain().actions();

	const auto printActionName =
		[&](const auto &action)
		{
			m_outputStream << utils::Keyword("action") << "(";

			if (action.parameters().empty())
			{
				m_outputStream << utils::String(action.name()) << ")";

				return;
			}

			m_outputStream << "(" << utils::String(action.name());
			this->translateVariablesHead(action.parameters());
			m_outputStream << "))";
		};

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			// TODO: rename
			const auto translateLiteral =
				[&](const auto &ruleHead, const auto &literal, bool enumerateEffects = false)
				{
					m_outputStream << std::endl << utils::RuleName(ruleHead) << "(";

					printActionName(*action);

					// TODO: implement conditional effects
					if (enumerateEffects)
						m_outputStream << ", " << utils::Keyword("effect") << "(" << utils::Reserved("unconditional") << ")";

					m_outputStream << ", ";

					this->translateLiteral(literal);

					m_outputStream << ") :- " << utils::RuleName("action") << "(";

					printActionName(*action);

					m_outputStream << ").";
				};

			m_outputStream << std::endl;

			// Name
			m_outputStream << utils::RuleName("action") << "(";
			printActionName(*action);
			m_outputStream << ")";

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
						throw utils::TranslatorException("only “and” expressions and (negated) predicates supported as action preconditions currently");

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
					translateLiteral("postcondition", effect, true);
				}
				// Assuming a conjunction
				else
				{
					if (effect.expressionType() != Expression::Type::And)
						throw utils::TranslatorException("only “and” expressions and (negated) predicates supported as action effects currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(effect);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto *argument)
						{
							translateLiteral("postcondition", *argument, true);
						});
				}
			}

			m_outputStream << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants(const std::string &heading, const expressions::Constants &constants) const
{
	m_outputStream << utils::Heading2(heading);

	std::for_each(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			m_outputStream << std::endl
				<< utils::RuleName("constant") << "("
				<< utils::Keyword("constant") << "("
				<< utils::String(constant->name())
				<< "))." << std::endl;

			const auto *type = constant->type();

			if (type != nullptr)
			{
				m_outputStream << utils::RuleName("has") << "("
					<< utils::Keyword("constant") << "(" << utils::String(constant->name()) << "), "
					<< utils::Keyword("type") << "(" << utils::String(type->name()) << "))." << std::endl;
			}
			else
			{
				m_outputStream << utils::RuleName("has") << "("
					<< utils::Keyword("constant") << "(" << utils::String(constant->name()) << "), "
					<< utils::Keyword("type") << "(" << utils::String("object") << "))." << std::endl;
			}
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariablesHead(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		const auto &variable = **i;

		m_outputStream << ", " << utils::ASPVariable(variable.name());
	}
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
				throw utils::TranslatorException("only primitive types supported currently");

			const auto &type = *dynamic_cast<const expressions::PrimitiveType *>(variable.type());

			m_outputStream << utils::RuleName("has") << "("
				<< utils::ASPVariable(variable.name()) << ", "
				<< utils::Keyword("type") << "(" << utils::String(type.name()) << "))";
		}
		else
		{
			m_outputStream << utils::RuleName("has") << "("
				<< utils::ASPVariable(variable.name()) << ", "
				<< utils::Keyword("type") << "(" << utils::String("object") << "))";
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateLiteral(const Expression &literal) const
{
	// Translate single predicate
	if (literal.expressionType() == Expression::Type::Predicate)
	{
		const auto &predicate = dynamic_cast<const expressions::Predicate &>(literal);

		m_outputStream << utils::Keyword("variable") << "(";
		this->translatePredicate(predicate);
		m_outputStream << "), " << utils::Keyword("value") << "(";
		this->translatePredicate(predicate);
		m_outputStream << ", " << utils::Boolean("true") << ")";
	}
	// Assuming that "not" expression may only contain a predicate
	else if (literal.expressionType() == Expression::Type::Not)
	{
		const auto &notExpression = dynamic_cast<const expressions::Not &>(literal);

		if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
			throw utils::TranslatorException("only negations of primitive predicates supported as literals currently");

		const auto &predicate = dynamic_cast<const expressions::Predicate &>(*notExpression.argument());

		m_outputStream << utils::Keyword("variable") << "(";
		this->translatePredicate(predicate);
		m_outputStream << "), " << utils::Keyword("value") << "(";
		this->translatePredicate(predicate);
		m_outputStream << ", " << utils::Boolean("false") << ")";
	}
	else
		throw utils::TranslatorException("only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicate(const expressions::Predicate &predicate) const
{
	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		m_outputStream << utils::String(predicate.name());

		return;
	}

	m_outputStream << "(" << utils::String(predicate.name());

	for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
	{
		m_outputStream << ", ";

		if ((*i)->expressionType() == Expression::Type::Constant)
		{
			const auto &constant = dynamic_cast<const expressions::Constant &>(**i);

			m_outputStream << utils::Keyword("constant") << "(" << utils::String(constant.name()) << ")";
		}
		else if ((*i)->expressionType() == Expression::Type::Variable)
		{
			const auto &variable = dynamic_cast<const expressions::Variable &>(**i);

			m_outputStream << utils::ASPVariable(variable.name());
		}
		else
			throw utils::TranslatorException("only variables and constants supported in predicates currently");
	}

	m_outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_outputStream << utils::Heading1("problem");

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

	m_outputStream << utils::Heading2("initial state");

	const auto &initialStateFacts = m_description.problem().initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << std::endl << utils::RuleName("initialState") << "(";

			// Translate single predicate
			if (fact->expressionType() == Expression::Type::Predicate)
			{
				const auto &predicate = dynamic_cast<const expressions::Predicate &>(*fact);

				m_outputStream << utils::Keyword("variable") << "(";
				this->translatePredicate(predicate);
				m_outputStream << "), " << utils::Keyword("value") << "(";
				this->translatePredicate(predicate);
				m_outputStream << ", " << utils::Boolean("true") << ")";
			}
			// Assuming that "not" expression may only contain a predicate
			else if (fact->expressionType() == Expression::Type::Not)
			{
				const auto &notExpression = dynamic_cast<const expressions::Not &>(*fact);

				if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
					throw utils::TranslatorException("only negations of simple predicates supported in initial state currently");
			}
			else
				throw utils::TranslatorException("only predicates and their negations supported in initial state currently");

			m_outputStream << ").";
		});

	m_outputStream
		<< std::endl << std::endl
		<< utils::RuleName("initialState") << "("
		<< utils::Keyword("variable") << "(" << utils::Variable("X") << "), "
		<< utils::Keyword("value") << "(" << utils::Variable("X") << ", " << utils::Boolean("false") << ")) :- "
		<< utils::RuleName("variable") << "(" << utils::Keyword("variable") << "(" << utils::Variable("X") << ")), "
		<< utils::Keyword("not") << " "
		<< utils::RuleName("initialState") << "("
		<< utils::Keyword("variable") << "(" << utils::Variable("X") << "), "
		<< utils::Keyword("value") << "(" << utils::Variable("X") << ", " << utils::Boolean("true") << "))."
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateGoal() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_outputStream << utils::Heading2("goal");

	const auto &goal = m_description.problem().goal();

	if (goal.expressionType() == Expression::Type::Predicate
		|| goal.expressionType() == Expression::Type::Not)
	{
		m_outputStream << std::endl << utils::RuleName("goal") << "(";

		translateLiteral(goal);

		m_outputStream << ").";
	}
	else if (goal.expressionType() == Expression::Type::And)
	{
		const auto &andExpression = dynamic_cast<const expressions::And &>(goal);

		std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
			[&](const auto *argument)
			{
				m_outputStream << std::endl << utils::RuleName("goal") << "(";

				this->translateLiteral(*argument);

				m_outputStream << ").";
			});
	}
	else
		throw utils::TranslatorException("only single predicates, their negations, and conjunctions are currently supported in the goal");

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
