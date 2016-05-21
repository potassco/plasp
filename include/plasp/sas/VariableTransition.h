#ifndef __SAS__VARIABLE_TRANSITION_H
#define __SAS__VARIABLE_TRANSITION_H

#include <iosfwd>

#include <plasp/sas/Value.h>
#include <plasp/sas/Variable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// VariableTransition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class VariableTransition
{
	public:
		static VariableTransition fromSAS(std::istream &istream, const std::vector<Variable> &variables);

	public:
		const Variable &variable() const;
		const Value &valueBefore() const;
		const Value &valueAfter() const;

	private:
		VariableTransition();

		const Variable *m_variable;
		const Value *m_valueBefore;
		const Value *m_valueAfter;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
