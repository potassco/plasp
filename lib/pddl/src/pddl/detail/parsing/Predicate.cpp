#include <pddl/detail/parsing/Predicate.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/detail/SignatureMatching.h>
#include <pddl/detail/parsing/Constant.h>
#include <pddl/detail/parsing/Variable.h>
#include <pddl/detail/parsing/VariableDeclaration.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::PredicatePointer> parsePredicate(Context &context, ASTContext &astContext, VariableStack &variableStack)
{
	auto &tokenizer = context.tokenizer;
	tokenizer.skipWhiteSpace();

	const auto previousPosition = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("("))
	{
		tokenizer.seek(previousPosition);
		return std::experimental::nullopt;
	}

	const auto name = tokenizer.getIdentifier();
	ast::Predicate::Arguments arguments;

	tokenizer.skipWhiteSpace();

	// Parse arguments
	while (tokenizer.currentCharacter() != ')')
	{
		// Parse argument if it is a variable
		auto variable = testParsingVariable(context, variableStack);

		if (variable)
		{
			arguments.emplace_back(std::move(variable.value()));

			tokenizer.skipWhiteSpace();
			continue;
		}

		// Parse argument if it is a constant
		auto constant = testParsingConstant(context, astContext);

		if (constant)
		{
			arguments.emplace_back(std::move(constant.value()));

			tokenizer.skipWhiteSpace();
			continue;
		}

		// If argument is neither variable nor constant, this is not a valid predicate
		tokenizer.seek(previousPosition);
		return std::experimental::nullopt;
	}

	const auto &predicates = astContext.domain->predicates;

	const auto matchingPredicateDeclaration = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicateDeclaration)
		{
			return matches(name, arguments, *predicateDeclaration);
		});

	if (matchingPredicateDeclaration == predicates.cend())
	{
		// TODO: enumerate candidates and why they are incompatible
		tokenizer.seek(previousPosition);
		throw ParserException(tokenizer.location(), "no matching declaration found for predicate “" + name + "”");
	}

	auto *declaration = matchingPredicateDeclaration->get();

	tokenizer.expect<std::string>(")");

	return std::make_unique<ast::Predicate>(std::move(arguments), declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
