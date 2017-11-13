#include <pddl/detail/Requirements.h>

#include <algorithm>

#include <pddl/Exception.h>
#include <pddl/detail/parsing/Requirement.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirements
//
////////////////////////////////////////////////////////////////////////////////////////////////////

bool hasRequirement(const ast::Domain &domain, ast::Requirement requirement)
{
	const auto match = std::find_if(domain.requirements.cbegin(), domain.requirements.cend(),
		[&](const auto &declaredRequirement)
		{
			return declaredRequirement == requirement;
		});

	return match != domain.requirements.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool hasRequirement(const ast::Problem &problem, ast::Requirement requirement)
{
	const auto match = std::find_if(problem.requirements.cbegin(), problem.requirements.cend(),
		[&](const auto &declaredRequirement)
		{
			return declaredRequirement == requirement;
		});

	if (match != problem.requirements.cend())
		return true;

	return hasRequirement(problem.domain, requirement);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool hasRequirement(const ASTContext &astContext, ast::Requirement requirement)
{
	if (astContext.problem)
		return hasRequirement(*astContext.problem.value(), requirement);

	return hasRequirement(*astContext.domain, requirement);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void checkRequirement(ast::Domain &domain, ast::Requirement requirement, Context &context)
{
	if (hasRequirement(domain, requirement))
		return;

	if (context.mode == Mode::Compatibility)
		context.warningCallback(context.tokenizer.location(), "requirement “" + std::string(toString(requirement)) + "” used but never declared, silently adding requirement");
	else
		throw ParserException(context.tokenizer.location(), "requirement “" + std::string(toString(requirement)) + "” used but never declared");

	domain.requirements.push_back(requirement);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void checkRequirement(ast::Problem &problem, ast::Requirement requirement, Context &context)
{
	if (hasRequirement(problem, requirement))
		return;

	if (context.mode == Mode::Compatibility)
		context.warningCallback(context.tokenizer.location(), "requirement “" + std::string(toString(requirement)) + "” used but never declared, silently adding requirement");
	else
		throw ParserException(context.tokenizer.location(), "requirement “" + std::string(toString(requirement)) + "” used but never declared");

	problem.requirements.push_back(requirement);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void checkRequirement(ASTContext &astContext, ast::Requirement requirement, Context &context)
{
	if (astContext.problem)
		checkRequirement(*astContext.problem.value(), requirement, context);
	else
		checkRequirement(*astContext.domain, requirement, context);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
