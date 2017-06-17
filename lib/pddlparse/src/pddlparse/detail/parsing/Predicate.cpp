#include <pddlparse/detail/parsing/Predicate.h>

#include <pddlparse/AST.h>
#include <pddlparse/detail/parsing/Constant.h>
#include <pddlparse/detail/parsing/Variable.h>
#include <pddlparse/detail/parsing/VariableDeclaration.h>

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

	const auto previousPosition = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("("))
	{
		tokenizer.seek(previousPosition);
		return std::experimental::nullopt;
	}

	const auto predicateName = tokenizer.getIdentifier();

	ast::Predicate::Arguments arguments;

	/*const auto matchingPredicate = std::find_if(predicates.cbegin(), predicates.cend(),
		[&](const auto &predicate)
		{
			return predicate->name() == predicateName;
		});

	if (matchingPredicate == predicates.cend())
	{
		tokenizer.seek(position);
		return std::experimental::nullopt;
	}

	auto predicate = PredicatePointer(new Predicate);

	predicate->m_name = predicateName;*/

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

	//const auto &predicates = astContext.domain->predicates;

	// TODO: check that signature matches one of the declared ones

	tokenizer.expect<std::string>(")");

	// TODO: add matching predicate declaration
	return std::make_unique<ast::Predicate>(std::move(arguments), nullptr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
