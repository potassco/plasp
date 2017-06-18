#ifndef __PDDL_PARSE__DETAIL__PARSING__DESCRIPTION_H
#define __PDDL_PARSE__DETAIL__PARSING__DESCRIPTION_H

#include <experimental/optional>

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>

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
