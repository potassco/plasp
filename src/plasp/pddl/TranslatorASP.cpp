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
		m_ostream << "% types";

		const auto &types = domain.types();

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

	// Constants
	if (!domain.constants().empty())
	{
		m_ostream << std::endl;
		m_ostream << "% constants";

		const auto &constants = domain.constants();

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

	// Predicates
	if (!domain.predicates().empty())
	{
		m_ostream << std::endl;
		m_ostream << "% predicates";

		const auto &predicates = domain.predicates();

		std::for_each(predicates.cbegin(), predicates.cend(),
			[&](const auto &predicate)
			{
				m_ostream << std::endl;

				m_ostream << "predicate(" << predicate->name();

				const auto &arguments = predicate->arguments();

				if (arguments.empty())
				{
					m_ostream << ").";
					return;
				}

				m_ostream << "(";

				for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
				{
					if (i != arguments.cbegin())
						m_ostream << ", ";

					m_ostream << utils::escapeASPVariable((*i)->name());
				}

				m_ostream << ")) :- ";

				for (auto i = arguments.cbegin(); i != arguments.cend(); i++)
				{
					if (i != arguments.cbegin())
						m_ostream << ", ";

					const auto &type = *dynamic_cast<const expressions::PrimitiveType *>((*i)->type());
					m_ostream << "hasType(" << utils::escapeASPVariable((*i)->name()) << ", type(" << type.name() << "))";
				}

				m_ostream << ".";
			});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem() const
{
	m_ostream << std::endl
		<< "%---------------------------------------" << std::endl
		<< "% problem" << std::endl
		<< "%---------------------------------------" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
