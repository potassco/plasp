#ifndef __PLASP__SAS__INITIAL_STATE_H
#define __PLASP__SAS__INITIAL_STATE_H

#include <tokenize/Tokenizer.h>

#include <plasp/sas/AssignedVariable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// InitialState
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class InitialState
{
	public:
		using Fact = AssignedVariable;
		using Facts = AssignedVariables;

		static InitialState fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables);

	public:
		const Facts &facts() const;

	private:
		InitialState() = default;

		Facts m_facts;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
