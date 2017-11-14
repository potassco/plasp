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

colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Description &description)
{
	// Metric section
	stream << "uses action costs: " << (description.usesActionCosts() ? "yes" : "no") << std::endl;

	// Variable section
	const auto &variables = description.variables();

	stream << "variables: " << variables.size() << std::endl;

	std::for_each(variables.cbegin(), variables.cend(),
		[&](const auto &variable)
		{
			const auto &values = variable.values();

			stream << "\t" << variable.name() << ":" << std::endl;
			stream << "\t\tvalues: " << values.size() << std::endl;

			std::for_each(values.cbegin(), values.cend(),
				[&](const auto &value)
				{
					stream << "\t\t\t";
					value.printAsSAS(stream);
					stream << std::endl;
				});

			stream << "\t\taxiom layer: " << variable.axiomLayer() << std::endl;
		});

	// Mutex section
	const auto &mutexGroups = description.mutexGroups();

	stream << "mutex groups: " << mutexGroups.size() << std::endl;

	std::for_each(mutexGroups.cbegin(), mutexGroups.cend(),
		[&](const auto &mutexGroup)
		{
			stream << "\tmutex group:" << std::endl;

			std::for_each(mutexGroup.facts().cbegin(), mutexGroup.facts().cend(),
				[&](const auto &fact)
				{
					stream << "\t\t" << fact.variable().name() << " = ";
					fact.value().printAsSAS(stream);
					stream << std::endl;
				});
		});

	// Initial state section
	const auto &initialState = description.initialState();

	stream << "initial state:" << std::endl;

	std::for_each(initialState.facts().cbegin(), initialState.facts().cend(),
		[&](const auto &fact)
		{
			stream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(stream);
			stream << std::endl;
		});

	// Goal section
	const auto &goal = description.goal();

	stream << "goal:" << std::endl;

	std::for_each(goal.facts().cbegin(), goal.facts().cend(),
		[&](const auto &fact)
		{
			stream << "\t" << fact.variable().name() << " = ";
			fact.value().printAsSAS(stream);
			stream << std::endl;
		});

	// Operator section
	const auto &operators = description.operators();

	stream << "operators: " << operators.size() << std::endl;

	std::for_each(operators.cbegin(), operators.cend(),
		[&](const auto &operator_)
		{
			stream << "\t";
			operator_.predicate().printAsSAS(stream);
			stream << ":" << std::endl;

			const auto &preconditions = operator_.preconditions();

			stream << "\t\tpreconditions: " << preconditions.size() << std::endl;

			std::for_each(preconditions.cbegin(), preconditions.cend(),
				[&](const auto &precondition)
				{
					std::cout << "\t\t\t" << precondition.variable().name() << " = ";
					precondition.value().printAsSAS(stream);
					stream << std::endl;
				});

			const auto &effects = operator_.effects();

			stream << "\t\teffects: " << effects.size() << std::endl;

			std::for_each(effects.cbegin(), effects.cend(),
				[&](const auto &effect)
				{
					stream << "\t\t\teffect:" << std::endl;

					const auto &conditions = effect.conditions();

					stream << "\t\t\t\tconditions: " << conditions.size() << std::endl;

					std::for_each(conditions.cbegin(), conditions.cend(),
						[&](const auto &condition)
						{
							stream << "\t\t\t\t\t" << condition.variable().name() << " = ";
							condition.value().printAsSAS(stream);
							stream << std::endl;
						});

					stream << "\t\t\t\tpostcondition:" << std::endl;
					stream << "\t\t\t\t\t" << effect.postcondition().variable().name() << " = ";
					effect.postcondition().value().printAsSAS(stream);
					stream << std::endl;
				});

			stream << "\t\tcosts: " << operator_.costs() << std::endl;
		});

	// Axiom section
	const auto &axiomRules = description.axiomRules();

	stream << "axiom rules: " << axiomRules.size() << std::endl;

	std::for_each(axiomRules.cbegin(), axiomRules.cend(),
		[&](const auto &axiomRule)
		{
			stream << "\taxiom rule:" << std::endl;

			const auto conditions = axiomRule.conditions();

			stream << "\t\tconditions: " << conditions.size() << std::endl;

			std::for_each(conditions.cbegin(), conditions.cend(),
				[&](const auto &condition)
				{
					stream << "\t\t\t" << condition.variable().name() << " = ";
					condition.value().printAsSAS(stream);
					stream << std::endl;
				});

			stream << "\t\tpostcondition:" << std::endl;
			stream << "\t\t\t" << axiomRule.postcondition().variable().name() << " = ";
			axiomRule.postcondition().value().printAsSAS(stream);
			stream << std::endl;
		});

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
