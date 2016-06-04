#ifndef __PLASP__PDDL__DOMAIN_H
#define __PLASP__PDDL__DOMAIN_H

#include <unordered_map>

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Requirement.h>

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
		static Domain fromPDDL(Context &context);

	public:
		const std::string &name() const;
		const Requirements &requirements() const;
		const expressions::PrimitiveTypes &types() const;
		const expressions::Constants &constants() const;
		const expressions::PredicateDeclarations &predicates() const;
		const std::vector<std::unique_ptr<Action>> &actions() const;

	private:
		Domain(Context &context);

		void parseSection();

		void parseRequirementSection();
		bool hasRequirement(Requirement::Type requirementType) const;
		void computeDerivedRequirements();

		void parseTypeSection();

		void parseConstantSection();

		void parsePredicateSection();

		void parseActionSection();

		void checkConsistency();

		Context &m_context;

		std::string m_name;
		Requirements m_requirements;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
