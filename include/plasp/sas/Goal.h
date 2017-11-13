#ifndef __PLASP__SAS__GOAL_H
#define __PLASP__SAS__GOAL_H

#include <tokenize/Tokenizer.h>

#include <plasp/sas/AssignedVariable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Goal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Goal
{
	public:
		using Fact = AssignedVariable;
		using Facts = AssignedVariables;

		static Goal fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables);

	public:
		const Facts &facts() const;

	private:
		Goal() = default;

		Facts m_facts;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
