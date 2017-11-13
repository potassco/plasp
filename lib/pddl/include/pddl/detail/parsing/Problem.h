#ifndef __PDDL__DETAIL__PARSING__PROBLEM_H
#define __PDDL__DETAIL__PARSING__PROBLEM_H

#include <pddl/AST.h>
#include <pddl/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ProblemParser
{
	public:
		ProblemParser(Context &context, ast::Domain &domain);
		ast::ProblemPointer parse();

	private:
		void findSections(ast::Problem &problem);

		void parseDomainSection(ast::Problem &problem);
		void parseRequirementSection(ast::Problem &problem);
		void computeDerivedRequirements(ast::Problem &problem);
		void parseObjectSection(ast::Problem &problem);
		void parseInitialStateSection(ast::Problem &problem);
		void parseGoalSection(ast::Problem &problem);

		Context &m_context;
		ast::Domain &m_domain;

		tokenize::StreamPosition m_domainPosition;
		tokenize::StreamPosition m_requirementsPosition;
		tokenize::StreamPosition m_objectsPosition;
		tokenize::StreamPosition m_initialStatePosition;
		tokenize::StreamPosition m_goalPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
