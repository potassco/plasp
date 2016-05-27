#ifndef __PLASP__SAS__EFFECT_H
#define __PLASP__SAS__EFFECT_H

#include <vector>

#include <plasp/sas/AssignedVariable.h>
#include <plasp/sas/Variable.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Effect
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Effect;
using Effects = std::vector<Effect>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Effect
{
	public:
		using Condition = AssignedVariable;
		using Conditions = AssignedVariables;

		static Effect fromSAS(utils::Parser &parser, const Variables &variables, Conditions &preconditions);

	public:
		const Conditions &conditions() const;
		const Condition &postcondition() const;

	private:
		Effect() = default;
		explicit Effect(Conditions conditions, Condition postcondition);

		Conditions m_conditions;
		Condition m_postcondition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
