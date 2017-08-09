#include <pddl/detail/SignatureMatching.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SignatureMatching
//
////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::PrimitiveTypeDeclaration &lhs, const ast::PrimitiveTypeDeclaration &rhs)
{
	// TODO: check if this assumption is correct
	// With typing enabled, all objects inherit from “object”
	if (rhs.name == "object")
		return true;

	// Two types match if rhs is lhs or one of its ancestors
	if (&lhs == &rhs)
		return true;

	for (const auto &lhsParentType : lhs.parentTypes)
		if (matches(*lhsParentType->declaration, rhs))
			return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::PrimitiveTypeDeclaration &lhs, const ast::Either<ast::PrimitiveTypePointer> &rhs)
{
	for (const auto &rhsType : rhs.arguments)
		if (matches(lhs, *rhsType->declaration))
			return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::Either<ast::PrimitiveTypePointer> &lhs, const ast::PrimitiveTypeDeclaration &rhs)
{
	if (lhs.arguments.size() != 1)
		return false;

	// Strictly speaking, a 1-ary either is identical to its argument
	return matches(*lhs.arguments[0]->declaration, rhs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::Either<ast::PrimitiveTypePointer> &lhs, const ast::Either<ast::PrimitiveTypePointer> &rhs)
{
	// All of the types in lhs must have a match in rhs
	for (const auto &lhsType : lhs.arguments)
		if (!matches(*lhsType->declaration, rhs))
			return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
bool matches(const T &lhs, const ast::Type &rhs)
{
	return rhs.match(
		[&](const ast::PrimitiveTypePointer &x){return matches(lhs, *x->declaration);},
		[&](const ast::EitherPointer<ast::PrimitiveTypePointer> &x){return matches(lhs, *x);});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::Type &lhs, const ast::Type &rhs)
{
	return lhs.match(
		[&](const ast::PrimitiveTypePointer &x){return matches(*x->declaration, rhs);},
		[&](const ast::EitherPointer<ast::PrimitiveTypePointer> &x){return matches(*x, rhs);});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::VariableDeclaration &lhs, const std::experimental::optional<ast::Type> &rhs)
{
	if (!lhs.type && !rhs)
		return true;

	// If typing is enabled, all objects have to be typed
	assert(lhs.type);
	assert(rhs);

	return matches(lhs.type.value(), rhs.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::ConstantDeclaration &lhs, const std::experimental::optional<ast::Type> &rhs)
{
	if (!lhs.type && !rhs)
		return true;

	// If typing is enabled, all objects have to be typed
	assert(lhs.type);
	assert(rhs);

	return matches(lhs.type.value(), rhs.value());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::Term &lhs, const std::experimental::optional<ast::Type> &rhs)
{
	return lhs.match([&](const auto &x){return matches(*x->declaration, rhs);});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const std::string &predicateName, const ast::Predicate::Arguments &predicateArguments, const ast::PredicateDeclaration &predicateDeclaration)
{
	if (predicateName != predicateDeclaration.name)
		return false;

	if (predicateArguments.size() != predicateDeclaration.parameters.size())
		return false;

	for (size_t i = 0; i < predicateArguments.size(); i++)
		if (!matches(predicateArguments[i], predicateDeclaration.parameters[i]->type))
			return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
