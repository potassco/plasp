#ifndef __PLASP__PDDL__DESCRIPTION_H
#define __PLASP__PDDL__DESCRIPTION_H

#include <boost/filesystem/path.hpp>

#include <plasp/pddl/Domain.h>
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
		static Description fromFile(const boost::filesystem::path &path);

	public:

	private:
		Description() = default;

		void parseContent(utils::Parser &parser);
		void parseSection(utils::Parser &parser);

		std::unique_ptr<Domain> m_domain;
		//std::unique_ptr<Problem> m_problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
