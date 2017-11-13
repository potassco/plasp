#include <pddl/detail/parsing/Requirement.h>

#include <cstring>
#include <map>

#include <pddl/AST.h>
#include <pddl/Exception.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CompareStrings
{
	bool operator()(const char *lhs, const char *rhs) const
	{
		return std::strcmp(lhs, rhs) < 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using RequirementNameMap = std::map<const char *, ast::Requirement, CompareStrings>;
static const RequirementNameMap requirementNameMap =
	{
		{"strips", ast::Requirement::STRIPS},
		{"typing", ast::Requirement::Typing},
		{"negative-preconditions", ast::Requirement::NegativePreconditions},
		{"disjunctive-preconditions", ast::Requirement::DisjunctivePreconditions},
		{"equality", ast::Requirement::Equality},
		{"existential-preconditions", ast::Requirement::ExistentialPreconditions},
		{"universal-preconditions", ast::Requirement::UniversalPreconditions},
		{"quantified-preconditions", ast::Requirement::QuantifiedPreconditions},
		{"conditional-effects", ast::Requirement::ConditionalEffects},
		{"fluents", ast::Requirement::Fluents},
		{"numeric-fluents", ast::Requirement::NumericFluents},
		{"object-fluents", ast::Requirement::ObjectFluents},
		{"adl", ast::Requirement::ADL},
		{"durative-actions", ast::Requirement::DurativeActions},
		{"duration-inequalities", ast::Requirement::DurationInequalities},
		{"continuous-effects", ast::Requirement::ContinuousEffects},
		{"derived-predicates", ast::Requirement::DerivedPredicates},
		{"timed-initial-literals", ast::Requirement::TimedInitialLiterals},
		{"preferences", ast::Requirement::Preferences},
		{"constraints", ast::Requirement::Constraints},
		{"action-costs", ast::Requirement::ActionCosts},
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::Requirement> parseRequirement(Context &context)
{
	auto &tokenizer = context.tokenizer;

	const auto requirementName = tokenizer.getIdentifier();

	const auto matchingRequirement = requirementNameMap.find(requirementName.c_str());

	if (matchingRequirement != requirementNameMap.cend())
		return matchingRequirement->second;

	if (context.mode == Mode::Compatibility && (requirementName == "goal-utilities" || requirementName == "domain-axioms"))
		context.warningCallback(tokenizer.location(), "“" + requirementName + "” requirement is not part of the PDDL 3.1 specification, ignoring requirement");

	return std::experimental::nullopt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *toString(const ast::Requirement &requirement)
{
	const auto matchingRequirement = std::find_if(requirementNameMap.cbegin(), requirementNameMap.cend(),
		[&](const auto &requirementNamePair)
		{
			return requirementNamePair.second == requirement;
		});

	assert(matchingRequirement != requirementNameMap.cend());

	return matchingRequirement->first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
