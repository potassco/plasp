#include <plasp/sas/TranslatorASP.h>

#include <plasp/sas/TranslatorException.h>
#include <plasp/utils/Parsing.h>

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
			const auto &effects = operator_.effects();

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					if (!effect.conditions().empty())
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

	ostream << "% feature requirements" << std::endl;

	if (m_description.usesActionCosts())
		ostream << "requiresFeature(actionCosts)." << std::endl;

	ostream << std::endl;
	ostream << "% initial state" << std::endl;

	const auto &initialStateFacts = m_description.initialState().facts();

	std::for_each(initialStateFacts.cbegin(), initialStateFacts.cend(),
		[&](const auto &fact)
		{
			if (fact.value() == Value::None)
				return;

			ostream << "initialState(";
			fact.variable().printNameAsASPPredicate(ostream);
			ostream << ", ";
			fact.value().printAsASPPredicate(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% goal" << std::endl;

	const auto &goalFacts = m_description.goal().facts();

	std::for_each(goalFacts.cbegin(), goalFacts.cend(),
		[&](const auto &fact)
		{
			if (fact.value() == Value::None)
				return;

			ostream << "goal(";
			fact.variable().printNameAsASPPredicate(ostream);
			ostream << ", ";
			fact.value().printAsASPPredicate(ostream);
			ostream << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% variables";

	const auto &variables = m_description.variables();

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			BOOST_ASSERT(!values.empty());

			ostream << std::endl;
			variable.printNameAsASPPredicate(ostream);
			ostream << "." << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					ostream << "contains(";
					variable.printNameAsASPPredicate(ostream);
					ostream << ", ";

					if (value == Value::None)
						ostream << "noneValue";
					else
						value.printAsASPPredicate(ostream);

					ostream << ")." << std::endl;
				});
		});

	ostream << std::endl;
	ostream << "% actions";

	const auto &operators = m_description.operators();

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			ostream << std::endl;
			operator_.printPredicateAsASP(ostream);
			ostream << "." << std::endl;

			const auto &preconditions = operator_.preconditions();

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					if (precondition.value() == Value::None)
						return;

					ostream << "precondition(";
					operator_.printPredicateAsASP(ostream);
					ostream << ", ";
					precondition.variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					precondition.value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
				});

			const auto &effects = operator_.effects();

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					if (effect.postcondition().value() == Value::None)
						return;

					ostream << "postcondition(";
					operator_.printPredicateAsASP(ostream);
					ostream << ", ";
					effect.postcondition().variable().printNameAsASPPredicate(ostream);
					ostream << ", ";
					effect.postcondition().value().printAsASPPredicate(ostream);
					ostream << ")." << std::endl;
				});

			ostream << "costs(";
			operator_.printPredicateAsASP(ostream);
			ostream << ", " << operator_.costs() << ")." << std::endl;
		});

	ostream << std::endl;
	ostream << "% mutex groups";

	const auto &mutexGroups = m_description.mutexGroups();

	for (size_t i = 0; i < mutexGroups.size(); i++)
	{
		const auto &mutexGroup = mutexGroups[i];

		ostream << std::endl << "mutexGroup(mutexGroup" << i << ")." << std::endl;

		const auto &facts = mutexGroup.facts();

		std::for_each(facts.cbegin(), facts.cend(),
			[&](const auto &fact)
			{
				ostream << "contains(mutexGroup(mutexGroup" << i << "), ";
				fact.variable().printNameAsASPPredicate(ostream);
				ostream << ", ";
				fact.value().printAsASPPredicate(ostream);
				ostream << ")." << std::endl;
			});
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
