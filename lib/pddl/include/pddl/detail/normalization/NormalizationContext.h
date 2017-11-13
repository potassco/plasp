#ifndef __PDDL__DETAIL__NORMALIZATION__NORMALIZATION_CONTEXT_H
#define __PDDL__DETAIL__NORMALIZATION__NORMALIZATION_CONTEXT_H

#include <pddl/NormalizedASTForward.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NormalizationContext
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct NormalizationContext
{
	NormalizationContext(normalizedAST::DerivedPredicateDeclarations &derivedPredicates)
	:	derivedPredicates{derivedPredicates}
	{
	}

	normalizedAST::DerivedPredicateDeclarations &derivedPredicates;
	size_t derivedPredicateIDStart = 1;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
