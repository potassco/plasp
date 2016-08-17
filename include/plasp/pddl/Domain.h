#ifndef __PLASP__PDDL__DOMAIN_H
#define __PLASP__PDDL__DOMAIN_H

#include <plasp/pddl/Action.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Parser.h>
#include <plasp/pddl/Requirement.h>
#include <plasp/pddl/expressions/Constant.h>
#include <plasp/pddl/expressions/PredicateDeclaration.h>
#include <plasp/pddl/expressions/PrimitiveType.h>

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
		void findSections();
		void parse();

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

		void normalizeParameterNames();

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

		utils::Stream::Position m_requirementsPosition;
		Requirements m_requirements;

		utils::Stream::Position m_typesPosition;
		expressions::PrimitiveTypes m_types;

		utils::Stream::Position m_constantsPosition;
		expressions::Constants m_constants;

		utils::Stream::Position m_predicatesPosition;
		expressions::PredicateDeclarations m_predicates;

		std::vector<utils::Stream::Position> m_actionPositions;
		std::vector<std::unique_ptr<Action>> m_actions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
