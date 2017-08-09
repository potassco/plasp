#include <pddl/detail/parsing/Unsupported.h>

#include <pddl/AST.h>
#include <pddl/detail/parsing/Utils.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Unsupported
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ParserException exceptUnsupportedExpression(tokenize::StreamPosition position, Context &context)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.seek(position);
	tokenizer.expect<std::string>("(");
	auto expressionType = tokenizer.getIdentifier();

	return ParserException(tokenizer.location(position), "“" + expressionType + "” expressions currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ParserException exceptUnsupportedSection(tokenize::StreamPosition position, Context &context)
{
	auto &tokenizer = context.tokenizer;

	tokenizer.seek(position);
	tokenizer.expect<std::string>("(");
	tokenizer.expect<std::string>(":");
	auto sectionType = tokenizer.getIdentifier();

	return ParserException(tokenizer.location(position), "“:" + sectionType + "” sections currently unsupported");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
