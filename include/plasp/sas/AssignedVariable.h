#ifndef __SAS__ASSIGNED_VARIABLE_H
#define __SAS__ASSIGNED_VARIABLE_H

#include <iosfwd>
#include <vector>

#include <plasp/sas/Value.h>
#include <plasp/sas/Variable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AssignedVariable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class AssignedVariable;
using AssignedVariables = std::vector<AssignedVariable>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class AssignedVariable
{
	public:
		static AssignedVariable fromSAS(std::istream &istream, const Variables &variables);
		static AssignedVariable fromSAS(std::istream &istream, const Variable &variable);

	public:
		AssignedVariable(const Variable &variable, const Value &value);

		const Variable &variable() const;
		const Value &value() const;

	private:
		AssignedVariable();

		const Variable *m_variable;
		const Value *m_value;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
