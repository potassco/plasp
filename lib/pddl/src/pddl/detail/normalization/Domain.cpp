#include <pddl/detail/normalization/Domain.h>

#include <pddl/AST.h>
#include <pddl/NormalizedAST.h>
#include <pddl/detail/normalization/Action.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

normalizedAST::DomainPointer normalize(ast::DomainPointer &&domain)
{
	auto normalizedDomain = std::make_unique<normalizedAST::Domain>();

	normalizedDomain->types = std::move(domain->types);
	normalizedDomain->name = std::move(domain->name);
	normalizedDomain->constants = std::move(domain->constants);
	normalizedDomain->predicates = std::move(domain->predicates);

	normalizedDomain->actions.reserve(domain->actions.size());

	NormalizationContext normalizationContext(normalizedDomain->derivedPredicates);

	for (auto &&action : domain->actions)
		normalizedDomain->actions.emplace_back(normalize(std::move(action), normalizationContext));

	return normalizedDomain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
