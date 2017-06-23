#include <pddlparse/detail/normalization/Domain.h>

#include <pddlparse/AST.h>
#include <pddlparse/NormalizedAST.h>
#include <pddlparse/detail/normalization/Action.h>

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

	for (auto &&action : domain->actions)
		normalizedDomain->actions.emplace_back(normalize(std::move(action), normalizedDomain->derivedPredicates));

	return normalizedDomain;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
