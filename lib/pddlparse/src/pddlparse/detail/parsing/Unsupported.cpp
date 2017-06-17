#include <pddlparse/detail/parsing/Unsupported.h>

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Unsupported
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ast::UnsupportedPointer parseUnsupported(Context &context)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.expect<std::string>("(");

	auto expressionType = tokenizer.getIdentifier();

	context.warningCallback(tokenizer.location(), "expression type “" + expressionType + "” currently unsupported in this context, substituting it with placeholder");

	skipSection(tokenizer);

	return std::make_unique<ast::Unsupported>(std::move(expressionType));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
