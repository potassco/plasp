#include <plasp/sas/Debugging.h>

#include <iostream>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Debugging
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator >>(std::ostream &ostream, const Description &description)
{
	// Metric section
	ostream << "uses action costs: " << (description.usesActionCosts() ? "yes" : "no") << std::endl;

	// Variable section
	const auto &variables = description.variables();

	ostream << "variables: " << variables.size() << std::endl;

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			ostream << "\t" << variable.name() << ":" << std::endl;
			ostream << "\t\tvalues: " << values.size() << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
			    {
					ostream << "\t\t\t";
					value.printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\taxiom layer: " << variable.axiomLayer() << std::endl;
		});

	// Mutex section
	const auto &mutexGroups = description.mutexGroups();

	ostream << "mutex groups: " << mutexGroups.size() << std::endl;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			ostream << "\tmutex group:" << std::endl;

			std::for_each(mutexGroup.facts().cbegin(), mutexGroup.facts().cend(),
				[&](const auto &fact)
				{
					ostream << "\t\t" << fact.variable().name() << " = ";
					fact.value().printAsSAS(ostream);
					ostream << std::endl;
				});
		});

	// Initial state section
	const auto &initialState = description.initialState();

	ostream << "initial state:" << std::endl;

	std::for_each(initialState.facts().cbegin(), initialState.facts().cend(),
		[&](const auto &fact)
		{
			ostream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(ostream);
			ostream << std::endl;
		});

	// Goal section
	const auto &goal = description.goal();

	ostream << "goal:" << std::endl;

	std::for_each(goal.facts().cbegin(), goal.facts().cend(),
		[&](const auto &fact)
		{
			ostream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(ostream);
			ostream << std::endl;
		});

	// Operator section
	const auto &operators = description.operators();

	ostream << "operators: " << operators.size() << std::endl;

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			ostream << "\t" << operator_.predicate() << ":" << std::endl;

			const auto &preconditions = operator_.preconditions();

			ostream << "\t\tpreconditions: " << preconditions.size() << std::endl;

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					std::cout << "\t\t\t" << precondition.variable().name() << " = ";
					precondition.value().printAsSAS(ostream);
					ostream << std::endl;
				});

			const auto &effects = operator_.effects();

			ostream << "\t\teffects: " << effects.size() << std::endl;

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					ostream << "\t\t\teffect:" << std::endl;

					const auto &conditions = effect.conditions();

					ostream << "\t\t\t\tconditions: " << conditions.size() << std::endl;

					std::for_each(conditions.cbegin(), conditions.cend(),
						[&](const auto &condition)
						{
							ostream << "\t\t\t\t\t" << condition.variable().name() << " = ";
							condition.value().printAsSAS(ostream);
							ostream << std::endl;
						});

					ostream << "\t\t\t\tpostcondition:" << std::endl;
					ostream << "\t\t\t\t\t" << effect.postcondition().variable().name() << " = ";
					effect.postcondition().value().printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\tcosts: " << operator_.costs() << std::endl;
		});

	// Axiom section
	const auto &axiomRules = description.axiomRules();

	ostream << "axiom rules: " << axiomRules.size() << std::endl;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			ostream << "\taxiom rule:" << std::endl;

			const auto conditions = axiomRule.conditions();

			ostream << "\t\tconditions: " << conditions.size() << std::endl;

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					ostream << "\t\t\t" << condition.variable().name() << " = ";
					condition.value().printAsSAS(ostream);
					ostream << std::endl;
				});

			ostream << "\t\tpostcondition:" << std::endl;
			ostream << "\t\t\t" << axiomRule.postcondition().variable().name() << " = ";
			axiomRule.postcondition().value().printAsSAS(ostream);
			ostream << std::endl;
		});

	return ostream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
