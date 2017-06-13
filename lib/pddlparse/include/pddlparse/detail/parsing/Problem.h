#ifndef __PDDL_PARSE__DETAIL__PARSING__PROBLEM_H
#define __PDDL_PARSE__DETAIL__PARSING__PROBLEM_H

#include <experimental/optional>

#include <pddlparse/AST.h>
#include <pddlparse/Context.h>

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

		tokenize::Stream::Position m_domainPosition;
		tokenize::Stream::Position m_requirementsPosition;
		tokenize::Stream::Position m_objectsPosition;
		tokenize::Stream::Position m_initialStatePosition;
		tokenize::Stream::Position m_goalPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
