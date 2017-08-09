#include <pddl/detail/normalization/Fact.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Literal.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Fact
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Fact normalize(ast::Fact &&fact)
{
	const auto handleLiteral =
		[&](ast::Literal &literal) -> normalizedAST::Literal
		{
			return normalize(std::move(literal));
		};

	const auto handleAt =
		[&](ast::AtPointer<ast::Literal> &) -> normalizedAST::Literal
		{
			throw NormalizationException("“at” expressions in preconditions can’t be normalized currently");
		};

	return fact.match(handleLiteral, handleAt);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
