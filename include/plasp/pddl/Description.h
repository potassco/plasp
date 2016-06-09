#ifndef __PLASP__PDDL__DESCRIPTION_H
#define __PLASP__PDDL__DESCRIPTION_H

#include <boost/filesystem/path.hpp>

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/Problem.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Description
{
	public:
		static Description fromParser(utils::Parser &&parser);
		static Description fromStream(std::istream &istream);
		static Description fromFile(const std::string &path);
		static Description fromFiles(const std::vector<std::string> &paths);

	public:
		const Domain &domain() const;
		const Problem &problem() const;

	private:
		Description();

		void parseContent();
		void findSections();

		void checkConsistency();

		utils::Parser m_parser;
		Context m_context;

		utils::Parser::Position m_domainPosition;
		std::unique_ptr<Domain> m_domain;
		utils::Parser::Position m_problemPosition;
		std::unique_ptr<Problem> m_problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
