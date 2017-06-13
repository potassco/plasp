#include <pddlparse/detail/parsing/Constant.h>

#include <pddlparse/AST.h>
#include <pddlparse/ParserException.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::experimental::optional<ast::ConstantPointer> findConstant(const std::string &constantName, ast::ConstantDeclarations &constantDeclarations)
{
	const auto matchingConstant = std::find_if(constantDeclarations.begin(), constantDeclarations.end(),
		[&](const auto &constantDeclaration)
		{
			return constantDeclaration->name == constantName;
		});

	if (matchingConstant == constantDeclarations.end())
		return std::experimental::nullopt;

	return std::make_unique<ast::Constant>(matchingConstant->get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::ConstantPointer parseConstant(Context &context, ast::Domain &domain)
{
	auto &tokenizer = context.tokenizer;
	const auto constantName = tokenizer.getIdentifier();

	auto constant = findConstant(constantName, domain.constants);

	if (constant)
		return std::move(constant.value());

	throw ParserException(tokenizer.location(), "constant “" + constantName + "” used but never declared");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::ConstantPointer parseConstant(Context &context, ast::Problem &problem)
{
	auto &tokenizer = context.tokenizer;
	const auto constantName = tokenizer.getIdentifier();

	auto constant = findConstant(constantName, problem.domain->constants);

	if (constant)
		return std::move(constant.value());

	auto object = findConstant(constantName, problem.objects);

	if (object)
		return std::move(object.value());

	throw ParserException(tokenizer.location(), "constant “" + constantName + "” used but never declared");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
