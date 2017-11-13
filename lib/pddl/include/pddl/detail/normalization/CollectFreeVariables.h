#ifndef __PDDL__DETAIL__NORMALIZATION__COLLECT_FREE_VARIABLES_H
#define __PDDL__DETAIL__NORMALIZATION__COLLECT_FREE_VARIABLES_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>
#include <pddl/Exception.h>

#include <pddl/NormalizedASTForward.h>
#include <pddl/detail/VariableStack.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Collect Free Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Variant>
void collectFreeVariables(const Variant &variant, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	variant.match([&](const auto &x){collectFreeVariables(x, freeVariables, variableStack);});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void collectFreeVariables(const ast::ConstantPointer &, std::vector<normalizedAST::VariableDeclaration *> &, VariableStack &)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void collectFreeVariables(const ast::VariablePointer &variable, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	if (variableStack.contains(*variable->declaration))
		return;

	const auto matchingFreeVariable = std::find(freeVariables.cbegin(), freeVariables.cend(), variable->declaration);

	if (matchingFreeVariable != freeVariables.cend())
		return;

	freeVariables.emplace_back(variable->declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void collectFreeVariables(const ast::PredicatePointer &predicate, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	for (const auto &argument : predicate->arguments)
		collectFreeVariables(argument, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::AndPointer<Argument> &and_, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	for (const auto &argument : and_->arguments)
		collectFreeVariables(argument, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ArgumentLeft, class ArgumentRight>
void collectFreeVariables(const ast::EqualsPointer<ArgumentLeft, ArgumentRight> &equals, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	collectFreeVariables(equals->argumentLeft, freeVariables, variableStack);
	collectFreeVariables(equals->argumentRight, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::ExistsPointer<Argument> &exists, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	variableStack.push(&exists->parameters);

	collectFreeVariables(exists->argument, freeVariables, variableStack);

	variableStack.pop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::ForAllPointer<Argument> &forAll, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	variableStack.push(&forAll->parameters);

	collectFreeVariables(forAll->argument, freeVariables, variableStack);

	variableStack.pop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::ImplyPointer<Argument> &imply, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	collectFreeVariables(imply->argumentLeft, freeVariables, variableStack);
	collectFreeVariables(imply->argumentRight, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::NotPointer<Argument> &not_, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	collectFreeVariables(not_->argument, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Argument>
void collectFreeVariables(const ast::OrPointer<Argument> &or_, std::vector<normalizedAST::VariableDeclaration *> &freeVariables, VariableStack &variableStack)
{
	for (const auto &argument : or_->arguments)
		collectFreeVariables(argument, freeVariables, variableStack);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
