#include <pddlparse/detail/normalization/Description.h>

#include <pddlparse/AST.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/Domain.h>
#include <pddlparse/detail/normalization/Problem.h>

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
