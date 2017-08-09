#ifndef __PDDL__DETAIL__SIGNATURE_MATCHING_H
#define __PDDL__DETAIL__SIGNATURE_MATCHING_H

#include <string>

#include <pddl/AST.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// SignatureMatching
//
////////////////////////////////////////////////////////////////////////////////////////////////////

bool matches(const ast::PrimitiveTypeDeclaration &lhs, const ast::PrimitiveTypeDeclaration &rhs);
bool matches(const ast::Either<ast::PrimitiveTypePointer> &lhs, const ast::PrimitiveTypeDeclaration &rhs);
bool matches(const ast::PrimitiveTypeDeclaration &lhs, const ast::Either<ast::PrimitiveTypePointer> &rhs);
bool matches(const ast::Either<ast::PrimitiveTypePointer> &lhs, const ast::Either<ast::PrimitiveTypePointer> &rhs);
bool matches(const ast::VariableDeclaration &lhs, const std::experimental::optional<ast::Type> &rhs);
bool matches(const ast::ConstantDeclaration &lhs, const std::experimental::optional<ast::Type> &rhs);
bool matches(const ast::Term &lhs, const std::experimental::optional<ast::Type> &rhs);

bool matches(const std::string &predicateName, const ast::Predicate::Arguments &predicateArguments, const ast::PredicateDeclaration &predicateDeclaration);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
