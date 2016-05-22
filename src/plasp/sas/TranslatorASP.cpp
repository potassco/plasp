#include <plasp/sas/TranslatorASP.h>

#include <plasp/sas/TranslatorException.h>

namespace plasp
{
namespace sas
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
	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			if (variable.axiomLayer() != -1)
				throw TranslatorException("Axiom layers are currently unsupported");
		});

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			std::for_each(operator_.effects.cbegin(), operator_.effects.cend(),
				[&](const auto &effect)
				{
					if (!effect.conditions.empty())
						throw TranslatorException("Conditional effects are currently unsupported");
				});
		});

	if (!m_description.axiomRules().empty())
		throw TranslatorException("Axiom rules are currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate(std::ostream &ostream) const
{
	checkSupport();

	std::vector<const std::string *> fluents;

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					const auto match = std::find_if(fluents.cbegin(), fluents.cend(),
						[&](const auto &fluent)
						{
							return value.name() == *fluent;
						});

					// Don’t add fluents if their negated form has already been added
					if (match != fluents.cend())
						return;

					fluents.push_back(&value.name());
				});
		});

	ostream << "% fluents" << std::endl;

	std::for_each(fluents.cbegin(), fluents.cend(),
		[&](const auto *fluent)
		{
			// TODO: Handle 0-ary predicates
			ostream << "fluent(" << *fluent << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% initial state" << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			ostream << "init(";
			fact.value().printAsASP(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% goal" << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			ostream << "goal(";
			fact.value().printAsASP(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% actions" << std::endl;

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			ostream << "action(" << operator_.predicate << ")." << std::endl;

			std::for_each(operator_.preconditions.cbegin(), operator_.preconditions.cend(),
				[&](const auto &precondition)
			    {
					ostream << "precondition(" << operator_.predicate
						<< ", " << precondition.value().name()
						<< ", " << (precondition.value().sign() == Value::Sign::Positive ? "true" : "false")
						<< ")." << std::endl;
				});

			std::for_each(operator_.effects.cbegin(), operator_.effects.cend(),
				[&](const auto &effect)
				{
					ostream << "postcondition(" << operator_.predicate
						<< ", " << effect.postcondition.value().name()
						<< ", " << (effect.postcondition.value().sign() == Value::Sign::Positive ? "true" : "false")
						<< ")." << std::endl;
				});
		});

	ostream << std::endl;
	ostream << "% mutex groups" << std::endl;

	const auto &mutexGroups = m_description.mutexGroups();

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			ostream << ":- time(T)";

			std::for_each(mutexGroup.facts().cbegin(), mutexGroup.facts().cend(),
				[&](const auto &fact)
				{
					ostream << ", holds(";
					fact.value().printAsASP(ostream);
					ostream << ", T)";
				});

			ostream << "." << std::endl;
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
