#include <plasp/pddl/TranslatorASP.h>

#include <boost/assert.hpp>

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

TranslatorASP::TranslatorASP(Description &description, output::ColorStream &outputStream)
:	m_description(description),
	m_outputStream(outputStream)
{
	m_description.normalize();
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
	m_outputStream << output::Heading1("domain");

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
	m_outputStream << output::Heading2("types");

	m_outputStream << std::endl;

	const auto &types = m_description.domain().types();

	if (types.empty())
	{
		m_outputStream
			<< output::Function("type") << "("
			<< output::Keyword("type") << "(" << output::String("object") << "))." << std::endl;

		return;
	}

	std::for_each(types.cbegin(), types.cend(),
		[&](const auto &type)
		{
			m_outputStream
				<< output::Function("type") << "("
				<< output::Keyword("type") << "("
				<< output::String(type->name().c_str())
				<< "))." << std::endl;

			const auto &parentTypes = type->parentTypes();

			std::for_each(parentTypes.cbegin(), parentTypes.cend(),
				[&](const auto &parentType)
				{
					m_outputStream
						<< output::Function("inherits") << "(" << output::Keyword("type")
						<< "(" << output::String(type->name().c_str()) << "), " << output::Keyword("type")
						<< "(" << output::String(parentType->name().c_str()) << "))." << std::endl;
				});
		});

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

	const auto &predicates = m_description.domain().predicates();

	const auto printPredicateName =
		[&](const auto &predicate)
		{
			if (predicate->arguments().empty())
			{
				m_outputStream << output::String(predicate->name().c_str());

				return;
			}

			m_outputStream << "(" << output::String(predicate->name().c_str());
			this->translateVariablesHead(predicate->arguments());
			m_outputStream << ")";
		};

	std::for_each(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			m_outputStream
				<< std::endl
				<< output::Function("variable") << "("
				<< output::Keyword("variable") << "(";

			printPredicateName(predicate);

			m_outputStream << "))";

			this->translateVariablesBody(predicate->arguments());

			m_outputStream << ".";
		});

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

	const auto &actions = m_description.domain().actions();

	const auto printActionName =
		[&](const auto &action)
		{
			m_outputStream << output::Keyword("action") << "(";

			if (action.parameters().empty())
			{
				m_outputStream << output::String(action.name().c_str()) << ")";

				return;
			}

			m_outputStream << "(" << output::String(action.name().c_str());
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
					m_outputStream << std::endl << output::Function(ruleHead) << "(";

					printActionName(*action);

					// TODO: implement conditional effects
					if (enumerateEffects)
						m_outputStream << ", " << output::Keyword("effect") << "(" << output::Reserved("unconditional") << ")";

					m_outputStream << ", ";

					this->translateLiteral(literal);

					m_outputStream << ") :- " << output::Function("action") << "(";

					printActionName(*action);

					m_outputStream << ").";
				};

			m_outputStream << std::endl;

			// Name
			m_outputStream << output::Function("action") << "(";
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
						throw output::TranslatorException("only “and” expressions and (negated) predicates supported as action preconditions currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(precondition);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto argument)
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
						throw output::TranslatorException("only “and” expressions and (negated) predicates supported as action effects currently");

					const auto &andExpression = dynamic_cast<const expressions::And &>(effect);

					std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
						[&](const auto argument)
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
	m_outputStream << output::Heading2(heading.c_str());

	std::for_each(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			m_outputStream << std::endl
				<< output::Function("constant") << "("
				<< output::Keyword("constant") << "("
				<< output::String(constant->name().c_str())
				<< "))." << std::endl;

			const auto type = constant->type();

			if (type != nullptr)
			{
				m_outputStream << output::Function("has") << "("
					<< output::Keyword("constant") << "(" << output::String(constant->name().c_str()) << "), "
					<< output::Keyword("type") << "(" << output::String(type->name().c_str()) << "))." << std::endl;
			}
			else
			{
				m_outputStream << output::Function("has") << "("
					<< output::Keyword("constant") << "(" << output::String(constant->name().c_str()) << "), "
					<< output::Keyword("type") << "(" << output::String("object") << "))." << std::endl;
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

		m_outputStream << ", " << output::Variable(variable.name().c_str());
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
				throw output::TranslatorException("only primitive types supported currently");

			const auto &type = dynamic_cast<const expressions::PrimitiveType &>(*variable.type());

			m_outputStream << output::Function("has") << "("
				<< output::Variable(variable.name().c_str()) << ", "
				<< output::Keyword("type") << "(" << output::String(type.name().c_str()) << "))";
		}
		else
		{
			m_outputStream << output::Function("has") << "("
				<< output::Variable(variable.name().c_str()) << ", "
				<< output::Keyword("type") << "(" << output::String("object") << "))";
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

		m_outputStream << output::Keyword("variable") << "(";
		this->translatePredicate(predicate);
		m_outputStream << "), " << output::Keyword("value") << "(";
		this->translatePredicate(predicate);
		m_outputStream << ", " << output::Boolean("true") << ")";
	}
	// Assuming that "not" expression may only contain a predicate
	else if (literal.expressionType() == Expression::Type::Not)
	{
		const auto &notExpression = dynamic_cast<const expressions::Not &>(literal);

		if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
			throw output::TranslatorException("only negations of primitive predicates supported as literals currently");

		const auto &predicate = dynamic_cast<const expressions::Predicate &>(*notExpression.argument());

		m_outputStream << output::Keyword("variable") << "(";
		this->translatePredicate(predicate);
		m_outputStream << "), " << output::Keyword("value") << "(";
		this->translatePredicate(predicate);
		m_outputStream << ", " << output::Boolean("false") << ")";
	}
	else
		throw output::TranslatorException("only primitive predicates and their negations supported as literals currently");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicate(const expressions::Predicate &predicate) const
{
	const auto &arguments = predicate.arguments();

	if (arguments.empty())
	{
		m_outputStream << output::String(predicate.name().c_str());

		return;
	}

	m_outputStream << "(" << output::String(predicate.name().c_str());

	for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
	{
		m_outputStream << ", ";

		if ((*i)->expressionType() == Expression::Type::Constant)
		{
			const auto &constant = dynamic_cast<const expressions::Constant &>(**i);

			m_outputStream << output::Keyword("constant") << "(" << output::String(constant.name().c_str()) << ")";
		}
		else if ((*i)->expressionType() == Expression::Type::Variable)
		{
			const auto &variable = dynamic_cast<const expressions::Variable &>(**i);

			m_outputStream << output::Variable(variable.name().c_str());
		}
		else
			throw output::TranslatorException("only variables and constants supported in predicates currently");
	}

	m_outputStream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	BOOST_ASSERT(m_description.containsProblem());

	m_outputStream << output::Heading1("problem");

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

	m_outputStream << output::Heading2("initial state");

	const auto &initialStateFacts = m_description.problem().initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			m_outputStream << std::endl << output::Function("initialState") << "(";

			// Translate single predicate
			if (fact->expressionType() == Expression::Type::Predicate)
			{
				const auto &predicate = dynamic_cast<const expressions::Predicate &>(*fact);

				m_outputStream << output::Keyword("variable") << "(";
				this->translatePredicate(predicate);
				m_outputStream << "), " << output::Keyword("value") << "(";
				this->translatePredicate(predicate);
				m_outputStream << ", " << output::Boolean("true") << ")";
			}
			// Assuming that "not" expression may only contain a predicate
			else if (fact->expressionType() == Expression::Type::Not)
			{
				const auto &notExpression = dynamic_cast<const expressions::Not &>(*fact);

				if (notExpression.argument()->expressionType() != Expression::Type::Predicate)
					throw output::TranslatorException("only negations of simple predicates supported in initial state currently");
			}
			else
				throw output::TranslatorException("only predicates and their negations supported in initial state currently");

			m_outputStream << ").";
		});

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
	BOOST_ASSERT(m_description.containsProblem());

	m_outputStream << output::Heading2("goal");

	const auto &goal = m_description.problem().goal();

	if (goal.expressionType() == Expression::Type::Predicate
		|| goal.expressionType() == Expression::Type::Not)
	{
		m_outputStream << std::endl << output::Function("goal") << "(";

		translateLiteral(goal);

		m_outputStream << ").";
	}
	else if (goal.expressionType() == Expression::Type::And)
	{
		const auto &andExpression = dynamic_cast<const expressions::And &>(goal);

		std::for_each(andExpression.arguments().cbegin(), andExpression.arguments().cend(),
			[&](const auto argument)
			{
				m_outputStream << std::endl << output::Function("goal") << "(";

				this->translateLiteral(*argument);

				m_outputStream << ").";
			});
	}
	else
		throw output::TranslatorException("only single predicates, their negations, and conjunctions are currently supported in the goal");

	m_outputStream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
