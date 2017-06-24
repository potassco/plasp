#ifndef __PDDL_PARSE__DETAIL__NORMALIZATION__COLLECT_FREE_VARIABLES_H
#define __PDDL_PARSE__DETAIL__NORMALIZATION__COLLECT_FREE_VARIABLES_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>
#include <pddlparse/Exception.h>

#include <pddlparse/NormalizedASTForward.h>
#include <pddlparse/detail/VariableStack.h>

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

inline void collectFreeVariables(const ast::UnsupportedPointer &unsupported, std::vector<normalizedAST::VariableDeclaration *> &, VariableStack &)
{
	throw NormalizationException("cannot collect free variables of unsupported “" + unsupported->type + "” expression");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
