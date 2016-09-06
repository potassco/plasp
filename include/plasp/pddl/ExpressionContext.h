#ifndef __PLASP__PDDL__EXPRESSION_CONTEXT_H
#define __PLASP__PDDL__EXPRESSION_CONTEXT_H

#include <plasp/pddl/Expression.h>
#include <plasp/pddl/Requirement.h>
#include <plasp/pddl/VariableStack.h>

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
		ExpressionContext(Domain &domain);
		ExpressionContext(Domain &domain, Problem *problem);

		bool hasRequirement(Requirement::Type requirementType) const;
		void checkRequirement(Requirement::Type requirementType) const;

		Domain &domain;
		Problem *problem;

		VariableStack variables;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
