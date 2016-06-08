#ifndef __PLASP__PDDL__EXPRESSION_CONTEXT_H
#define __PLASP__PDDL__EXPRESSION_CONTEXT_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Requirement.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ExpressionContext
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ExpressionContext
{
	public:
		ExpressionContext(Domain &domain, expressions::Variables &parameters);
		ExpressionContext(Domain &domain, Problem *problem, expressions::Variables &parameters);

		bool hasRequirement(Requirement::Type requirementType) const;
		void checkRequirement(Requirement::Type requirementType) const;

		Domain &domain;
		Problem *problem;

		expressions::Variables &parameters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
