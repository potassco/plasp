#include <plasp/pddl/TranslatorASP.h>

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
	// Check for "either" types
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

			std::for_each(parameters.cbegin(), parameters.cend(),
				[&](const auto &parameter)
				{
					if (parameter->type()->expressionType() != Expression::Type::PrimitiveType)
						throw utils::TranslatorException("Only primitive types supported currently");
				});
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

			translateVariables(predicate->arguments());

			m_ostream << ").";
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateActions() const
{
	m_ostream << "% actions";

	const auto &actions = m_description.domain().actions();

	std::for_each(actions.cbegin(), actions.cend(),
		[&](const auto &action)
		{
			m_ostream << std::endl;

			m_ostream << "action(" << action->name();

			translateVariables(action->parameters());

			m_ostream << ").";
		});

	m_ostream << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateVariables(const expressions::Variables &variables) const
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

	m_ostream << ")) :- ";

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

void TranslatorASP::translateProblem() const
{
	m_ostream
		<< "%---------------------------------------" << std::endl
		<< "% problem" << std::endl
		<< "%---------------------------------------" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
