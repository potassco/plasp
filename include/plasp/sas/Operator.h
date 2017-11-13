#ifndef __PLASP__SAS__OPERATOR_H
#define __PLASP__SAS__OPERATOR_H

#include <string>
#include <vector>

#include <tokenize/Tokenizer.h>

#include <colorlog/ColorStream.h>

#include <plasp/sas/AssignedVariable.h>
#include <plasp/sas/Effect.h>
#include <plasp/sas/Predicate.h>
#include <plasp/sas/Variable.h>


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

		static Operator fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables);

	public:
		void printPredicateAsASP(colorlog::ColorStream &stream) const;

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
