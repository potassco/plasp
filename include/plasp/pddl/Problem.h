#ifndef __PLASP__PDDL__PROBLEM_H
#define __PLASP__PDDL__PROBLEM_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Requirement.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Problem
{
	public:
		static Problem fromPDDL(Context &context);

	public:
		const std::string &name() const;
		const Requirements &requirements() const;

	private:
		Problem(Context &context);

		void parseSection();

		void parseRequirementSection();
		bool hasRequirement(Requirement::Type requirementType) const;
		void computeDerivedRequirements();

		void checkConsistency();

		Context &m_context;

		std::string m_name;
		Requirements m_requirements;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
