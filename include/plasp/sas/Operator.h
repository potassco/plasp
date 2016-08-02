#ifndef __PLASP__SAS__OPERATOR_H
#define __PLASP__SAS__OPERATOR_H

#include <string>
#include <vector>

#include <plasp/sas/AssignedVariable.h>
#include <plasp/sas/Effect.h>
#include <plasp/sas/Predicate.h>
#include <plasp/sas/Variable.h>
#include <plasp/utils/LogStream.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Operator
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Operator;
using Operators = std::vector<Operator>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Operator
{
	public:
		using Condition = AssignedVariable;
		using Conditions = AssignedVariables;

		static Operator fromSAS(utils::Parser<> &parser, const Variables &variables);

	public:
		void printPredicateAsASP(utils::LogStream &ostream) const;

		const Predicate &predicate() const;
		const Conditions &preconditions() const;
		const Effects &effects() const;
		size_t costs() const;

	private:
		Operator() = default;

		Predicate m_predicate;
		Conditions m_preconditions;
		Effects m_effects;
		size_t m_costs;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
