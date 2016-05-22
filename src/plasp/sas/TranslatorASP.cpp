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
	if (m_description.usesActionCosts())
		throw TranslatorException("Action costs are currently unsupported");

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
			const auto &effects = operator_.effects();

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					if (!effect.conditions().empty())
						throw TranslatorException("Conditional effects are currently unsupported");
				});

			if (operator_.costs() != 1)
				throw TranslatorException("Action costs are currently unsupported");
		});

	if (!m_description.axiomRules().empty())
		throw TranslatorException("Axiom rules are currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void TranslatorASP::translate(std::ostream &ostream) const
{
	checkSupport();

	ostream << "#program base." << std::endl << std::endl;

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

	ostream << "% initial state" << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			if (fact.value().sign() == Value::Sign::Negative)
				return;

			ostream << "initialState(";
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
			fact.value().printAsASPPredicateBody(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% fluents" << std::endl;

	std::for_each(fluents.cbegin(), fluents.cend(),
		[&](const auto *fluent)
		{
			ostream << "fluent(" << *fluent << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% actions" << std::endl;

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			ostream << "action(";
			operator_.predicate().printAsASP(ostream);
			ostream << ")." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					ostream << "precondition(";
					operator_.predicate().printAsASP(ostream);
					ostream << ", ";
					precondition.value().printAsASPPredicateBody(ostream);
					ostream << ")." << std::endl;
				});

			const auto &effects = operator_.effects();

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					ostream << "postcondition(";
					operator_.predicate().printAsASP(ostream);
					ostream << ", ";
					effect.postcondition().value().printAsASPPredicateBody(ostream);
					ostream << ")." << std::endl;
				});

			ostream << std::endl;
		});

	ostream << "#program step(t)." << std::endl << std::endl;

	ostream << "% constraints derived from SAS variables" << std::endl;

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			// Skip trivial constraints of the form :- x, not x.
			if (values.size() == 2 && values[0].name() == values[1].name())
				return;

			for (auto i = values.cbegin(); i != values.cend(); i++)
				for (auto j = i + 1; j != values.cend(); j++)
				{
					const auto &value1 = *i;
					const auto &value2 = *j;

					ostream << "mutex(";
					value1.printAsASPPredicateBody(ostream);
					ostream << ", ";
					value2.printAsASPPredicateBody(ostream);
					ostream << ")." << std::endl;
				}

			ostream << ":- ";

			for (auto i = values.cbegin(); i != values.cend(); i++)
			{
				const auto &value = *i;

				if (i != values.cbegin())
					ostream << ", ";

				ostream << "holds(";
				value.negated().printAsASPPredicateBody(ostream);
				ostream << ", t)";
			}

			ostream << "." << std::endl;
		});

	ostream << std::endl;
	ostream << "% constraints derived from SAS mutex groups" << std::endl;

	const auto &mutexGroups = m_description.mutexGroups();

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			const auto &facts = mutexGroup.facts();

			for (auto i = facts.cbegin(); i != facts.cend(); i++)
				for (auto j = i + 1; j != facts.cend(); j++)
				{
					const auto &value1 = i->value();
					const auto &value2 = j->value();

					ostream << "mutex(";
					value1.printAsASPPredicateBody(ostream);
					ostream << ", ";
					value2.printAsASPPredicateBody(ostream);
					ostream << ")." << std::endl;
				}
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}