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
		Problem(Context &context, Domain &domain);

		void readPDDL();

		bool isDeclared() const;

		Domain &domain();
		const Domain &domain() const;

		const std::string &name() const;

		const Requirements &requirements() const;

		expressions::Constants &objects();
		const expressions::Constants &objects() const;

		void checkConsistency();

	private:
		void parseSection();

		void parseRequirementSection();
		bool hasRequirement(Requirement::Type requirementType) const;
		void computeDerivedRequirements();

		void parseObjectSection();

		Context &m_context;
		Domain &m_domain;
		bool m_isDeclared;

		std::string m_name;
		Requirements m_requirements;

		expressions::Constants m_objects;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
