#ifndef __PLASP__SAS__MUTEX_GROUP_H
#define __PLASP__SAS__MUTEX_GROUP_H

#include <vector>

#include <plasp/sas/AssignedVariable.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MutexGroup
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class MutexGroup;
using MutexGroups = std::vector<MutexGroup>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class MutexGroup
{
	public:
		using Fact = AssignedVariable;
		using Facts = AssignedVariables;

		static MutexGroup fromSAS(std::istream &istream, const Variables &variables);

	public:
		const Facts &facts() const;

	private:
		MutexGroup() = default;

		Facts m_facts;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
