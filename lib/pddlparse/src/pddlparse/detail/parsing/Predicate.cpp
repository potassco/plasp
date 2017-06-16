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

	const auto position = tokenizer.position();

	if (!tokenizer.testAndSkip<std::string>("("))
	{
		tokenizer.seek(position);
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
		// Parse variables
		if (tokenizer.currentCharacter() == '?')
			arguments.emplace_back(parseVariable(context, variableStack));
		// Parse constants
		else
			arguments.emplace_back(parseConstant(context, astContext));

		tokenizer.skipWhiteSpace();
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
