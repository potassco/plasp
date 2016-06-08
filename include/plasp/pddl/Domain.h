#ifndef __PLASP__PDDL__DOMAIN_H
#define __PLASP__PDDL__DOMAIN_H

#include <plasp/pddl/Action.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
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
		Domain(Context &context);

	public:
		void readPDDL();

		void setName(std::string name);
		const std::string &name() const;

		const Requirements &requirements() const;
		bool hasRequirement(Requirement::Type requirementType) const;
		void checkRequirement(Requirement::Type requirementType) const;

		expressions::PrimitiveTypes &types();
		const expressions::PrimitiveTypes &types() const;

		expressions::Constants &constants();
		const expressions::Constants &constants() const;

		expressions::PredicateDeclarations &predicates();
		const expressions::PredicateDeclarations &predicates() const;

		std::vector<std::unique_ptr<Action>> &actions();
		const std::vector<std::unique_ptr<Action>> &actions() const;

		void checkConsistency();

	private:
		void parseSection();

		void parseRequirementSection();
		void computeDerivedRequirements();

		void parseTypeSection();

		void parseConstantSection();

		void parsePredicateSection();

		void parseActionSection();

		Context &m_context;

		std::string m_name;
		Requirements m_requirements;
		expressions::PrimitiveTypes m_primitiveTypes;
		expressions::Constants m_constants;
		expressions::PredicateDeclarations m_predicateDeclarations;
		std::vector<std::unique_ptr<Action>> m_actions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
