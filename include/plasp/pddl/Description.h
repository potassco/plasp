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
		static Description fromStream(std::istream &istream);
		static Description fromFiles(const std::vector<std::string> &paths);

	public:
		const Domain &domain() const;

	private:
		Description(std::istream &istream);

		void parseContent();
		void parseSection();

		void checkConsistency();

		utils::Parser m_parser;
		Context m_context;

		std::unique_ptr<Domain> m_domain;
		std::unique_ptr<Problem> m_problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
