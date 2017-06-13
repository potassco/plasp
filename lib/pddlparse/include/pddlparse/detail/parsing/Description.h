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
		tokenize::Stream::Position m_domainPosition;
		tokenize::Stream::Position m_problemPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
