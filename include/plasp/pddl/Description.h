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
		static Description fromContext(Context &&context);
		static Description fromStream(std::istream &istream);
		static Description fromFile(const std::string &path);
		static Description fromFiles(const std::vector<std::string> &paths);

	public:
		Context &context();
		const Context &context() const;

		const Domain &domain() const;

		bool containsProblem() const;
		const Problem &problem() const;

		void normalizeParameterNames();

	private:
		Description();

		void parse();
		void findSections();

		void checkConsistency();

		Context m_context;

		utils::Stream::Position m_domainPosition;
		std::unique_ptr<Domain> m_domain;
		utils::Stream::Position m_problemPosition;
		std::unique_ptr<Problem> m_problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
