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

void TranslatorASP::checkSupport() const
{
	// Check for "either" types in predicate declarations
	const auto &predicates = m_description.domain().predicates();

	std::for_each(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			const auto &arguments = predicate->arguments();

			std::for_each(arguments.cbegin(), arguments.cend(),
				[&](const auto &parameter)
				{
					if (parameter->type()->expressionType() != Expression::Type::PrimitiveType)
						throw utils::TranslatorException("Only primitive types supported currently");
				});
		});

	const auto &actions = m_description.domain().actions();

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			const auto &parameters = action->parameters();

			// Check for "either" types in action parameters
			std::for_each(parameters.cbegin(), parameters.cend(),
				[&](const auto &parameter)
				{
					if (parameter->type()->expressionType() != Expression::Type::PrimitiveType)
						throw utils::TranslatorException("Only primitive types supported currently");
				});

			if (action->precondition())
			{
				// Check that all preconditions are "and" expressions or single predicates
				if (action->precondition()->expressionType() != Expression::Type::And
				    && action->precondition()->expressionType() != Expression::Type::Predicate)
				{
					throw utils::TranslatorException("Only \"and\" expressions and single predicates supported as action preconditions currently");
				}

				// Check that "and" expression in preconditions contains single predicates only
				if (action->precondition()->expressionType() == Expression::Type::And)
				{
					const auto &precondition = dynamic_cast<const expressions::And &>(*action->precondition());
					const auto &preconditionArguments = precondition.arguments();

					std::for_each(preconditionArguments.cbegin(), preconditionArguments.cend(),
						[&](const auto &argument)
						{
							if (argument->expressionType() != Expression::Type::Predicate)
								throw utils::TranslatorException("Only predicates supported in preconditions currently");
						});
				}
			}

			if (action->effect())
			{
				// Check that all effects are "and" expressions
				if (action->effect()->expressionType() != Expression::Type::And
				    && action->effect()->expressionType() != Expression::Type::Predicate)
				{
					throw utils::TranslatorException("Only \"and\" expressions and single predicates supported as action effects currently");
				}

				// Check that "and" expression in effect contains single predicates or negated predicates only
				if (action->effect()->expressionType() == Expression::Type::And)
				{
					const auto &effect = dynamic_cast<const expressions::And &>(*action->effect());
					const auto &effectArguments = effect.arguments();

					std::for_each(effectArguments.cbegin(), effectArguments.cend(),
						[&](const auto *argument)
						{
							if (argument->expressionType() == Expression::Type::Not)
							{
								const auto &notExpression = dynamic_cast<const expressions::Not &>(*argument);
								argument = notExpression.argument();
							}

							if (argument->expressionType() != Expression::Type::Predicate)
								throw utils::TranslatorException("Only predicates and negated predicates supported in effects currently");
						});
				}
			}
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate() const
{
	checkSupport();

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
	if (!domain.types().empty())
	{
		m_ostream << std::endl;
		translateTypes();
	}

	// Constants
	if (!domain.constants().empty())
	{
		m_ostream << std::endl;
		translateConstants();
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
	// TODO: escape ASP identifiers
	m_ostream << "% types";

	const auto &types = m_description.domain().types();

	std::for_each(types.cbegin(), types.cend(),
		[&](const auto &type)
		{
			m_ostream << std::endl;

			m_ostream << "type(" << type->name() << ")." << std::endl;

			const auto &parentTypes = type->parentTypes();

			std::for_each(parentTypes.cbegin(), parentTypes.cend(),
				[&](const auto &parentType)
				{
					m_ostream << "inherits(type(" << type->name() << "), type(" << parentType->name() << "))." << std::endl;
				});
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateConstants() const
{
	m_ostream << "% constants";

	const auto &constants = m_description.domain().constants();

	std::for_each(constants.cbegin(), constants.cend(),
		[&](const auto &constant)
		{
			m_ostream << std::endl;

			m_ostream << "constant(" << constant->name() << ")." << std::endl;

			const auto *type = constant->type();

			if (type == nullptr)
				return;

			m_ostream << "hasType(constant(" << constant->name() << "), type(" << type->name() << "))." << std::endl;
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

			m_ostream << "predicate(" << predicate->name();

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
			m_ostream << "action(" << action.name();

			this->translateVariablesHead(action.parameters());

			m_ostream << ")";
		};

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			m_ostream << std::endl;

			// Name
			printActionName(*action);

			this->translateVariablesBody(action->parameters());

			m_ostream << ".";

			const auto translateLiteral =
				[&](const auto &ruleHead, const auto &literal)
				{
					m_ostream << std::endl;
					m_ostream << ruleHead << "(";

					printActionName(*action);

					m_ostream << ", ";

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

					m_ostream << ") :- ";

					printActionName(*action);

					m_ostream << ".";
				};

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

void TranslatorASP::translateVariablesHead(const expressions::Variables &variables) const
{
	if (variables.empty())
		return;

	m_ostream << "(";

	for (auto i = variables.cbegin(); i != variables.cend(); i++)
	{
		if (i != variables.cbegin())
			m_ostream << ", ";

		const auto &variable = *dynamic_cast<const expressions::Variable *>(i->get());

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
		if (i != variables.cbegin())
			m_ostream << ", ";

		const auto &variable = *dynamic_cast<const expressions::Variable *>(i->get());
		const auto &type = *dynamic_cast<const expressions::PrimitiveType *>(variable.type());

		m_ostream << "hasType(" << utils::escapeASPVariable(variable.name()) << ", type(" << type.name() << "))";
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translatePredicate(const expressions::Predicate &predicate) const
{
	m_ostream << "predicate(" << predicate.name();

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

			m_ostream << "constant(" << constant.name() << ")";
		}
		else if ((*i)->expressionType() == Expression::Type::Variable)
		{
			const auto &variable = dynamic_cast<const expressions::Variable &>(**i);

			m_ostream << utils::escapeASPVariable(variable.name());
		}
		else
			throw utils::TranslatorException("Only variables and constants supported in predicates");
	}

	m_ostream << "))";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	m_ostream
		<< "%---------------------------------------" << std::endl
		<< "% problem" << std::endl
		<< "%---------------------------------------" << std::endl;

	BOOST_ASSERT(m_description.containsProblem());

	const auto &problem = m_description.problem();

	// Objects
	if (!problem.objects().empty())
	{
		m_ostream << std::endl;
		translateObjects();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateObjects() const
{
	m_ostream << "% objects";

	const auto &objects = m_description.problem().objects();

	std::for_each(objects.cbegin(), objects.cend(),
		[&](const auto &object)
		{
			m_ostream << std::endl;

			m_ostream << "object(" << object->name() << ")." << std::endl;

			const auto *type = object->type();

			if (type == nullptr)
				return;

			m_ostream << "hasType(object(" << object->name() << "), type(" << type->name() << "))." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
