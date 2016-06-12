#include <plasp/pddl/TranslatorASP.h>

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

TranslatorASP::TranslatorASP(const Description &description)
:	m_description(description)
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

void TranslatorASP::translate(std::ostream &ostream) const
{
	checkSupport();

	translateDomain(ostream);

	if (m_description.containsProblem())
		translateProblem(ostream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateDomain(std::ostream &ostream) const
{
	ostream
		<< "%---------------------------------------" << std::endl
		<< "% domain" << std::endl
		<< "%---------------------------------------" << std::endl;

	const auto &domain = m_description.domain();

	// Types
	if (!domain.types().empty())
	{
		ostream << std::endl;
		ostream << "% types";

		const auto &types = domain.types();

		std::for_each(types.cbegin(), types.cend(),
			[&](const auto &type)
			{
				ostream << std::endl;

				ostream << "type(" << type->name() << ")." << std::endl;

				const auto &parentTypes = type->parentTypes();

				std::for_each(parentTypes.cbegin(), parentTypes.cend(),
					[&](const auto &parentType)
					{
						ostream << "inherits(type(" << type->name() << "), type(" << parentType->name() << "))." << std::endl;
					});
			});
	}

	// Constants
	if (!domain.constants().empty())
	{
		ostream << std::endl;
		ostream << "% constants";

		const auto &constants = domain.constants();

		std::for_each(constants.cbegin(), constants.cend(),
			[&](const auto &constant)
			{
				ostream << std::endl;

				ostream << "constant(" << constant->name() << ")." << std::endl;

				const auto *type = constant->type();

				if (type == nullptr)
					return;

				ostream << "hasType(constant(" << constant->name() << "), type(" << type->name() << "))." << std::endl;
			});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translateProblem(std::ostream &ostream) const
{
	ostream << std::endl
		<< "%---------------------------------------" << std::endl
		<< "% problem" << std::endl
		<< "%---------------------------------------" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
