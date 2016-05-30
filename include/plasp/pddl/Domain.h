#ifndef __PLASP__PDDL__DOMAIN_H
#define __PLASP__PDDL__DOMAIN_H

#include <plasp/pddl/Requirement.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Domain
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Domain
{
	public:
		static Domain fromPDDL(utils::Parser &parser);

	public:
		const std::string &name() const;
		const Requirement::Types &requirements() const;

	private:
		Domain() = default;

		void parseSection(utils::Parser &parser);

		void parseRequirementsSection(utils::Parser &parser);
		void computeDerivedRequirements();

		std::string m_name;
		Requirement::Types m_requirements;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
