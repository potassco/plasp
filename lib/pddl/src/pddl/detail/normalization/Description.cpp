#include <pddl/detail/normalization/Description.h>

#include <pddl/AST.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Domain.h>
#include <pddl/detail/normalization/Problem.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::Description normalize(ast::Description &&description)
{
	normalizedAST::Description normalizedDescription;

	normalizedDescription.domain = normalize(std::move(description.domain));

	if (description.problem)
		normalizedDescription.problem = normalize(std::move(description.problem.value()), normalizedDescription.domain.get());

	return normalizedDescription;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
