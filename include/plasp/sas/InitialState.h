#ifndef __SAS__INITIAL_STATE_H
#define __SAS__INITIAL_STATE_H

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
		using Facts = std::vector<Fact>;

		static InitialState fromSAS(std::istream &istream, const std::vector<Variable> &variables);

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
