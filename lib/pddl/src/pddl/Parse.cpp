#include <pddl/Parse.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/Description.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parse
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Description parseDescription(Context &context)
{
	return detail::DescriptionParser(context).parse();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
