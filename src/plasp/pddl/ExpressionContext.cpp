#include <plasp/pddl/ExpressionContext.h>

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/Problem.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ExpressionContext
//
////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionContext::ExpressionContext(Domain &domain, expressions::Variables &parameters)
:	domain(domain),
	problem(nullptr),
	parameters(parameters)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionContext::ExpressionContext(Domain &domain, Problem *problem, expressions::Variables &parameters)
:	domain(domain),
	problem{problem},
	parameters(parameters)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool ExpressionContext::hasRequirement(Requirement::Type requirementType) const
{
	if (problem != nullptr)
		return problem->hasRequirement(requirementType);

	return domain.hasRequirement(requirementType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void ExpressionContext::checkRequirement(Requirement::Type requirementType) const
{
	if (problem != nullptr)
		problem->checkRequirement(requirementType);
	else
		domain.checkRequirement(requirementType);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
