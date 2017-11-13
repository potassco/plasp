#ifndef __PDDL__DETAIL__PARSING__DESCRIPTION_H
#define __PDDL__DETAIL__PARSING__DESCRIPTION_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class DescriptionParser
{
	public:
		DescriptionParser(Context &context);
		ast::Description parse();

	private:
		void findSections();

		Context &m_context;
		tokenize::StreamPosition m_domainPosition;
		tokenize::StreamPosition m_problemPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
