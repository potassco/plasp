#ifndef __PLASP__PDDL__TRANSLATION__PRIMITIVES_H
#define __PLASP__PDDL__TRANSLATION__PRIMITIVES_H

#include <colorlog/Formatting.h>

#include <pddl/NormalizedAST.h>

#include <plasp/TranslatorException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Primitives
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::ConstantDeclaration &constantDeclaration)
{
	assert(!constantDeclaration.name.empty());

	return (stream << colorlog::String(constantDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::Constant &constant)
{
	assert(constant.declaration != nullptr);

	return (stream << *constant.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::PrimitiveTypeDeclaration &primitiveTypeDeclaration)
{
	assert(!primitiveTypeDeclaration.name.empty());

	return (stream << colorlog::String(primitiveTypeDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::PrimitiveType &primitiveType)
{
	assert(primitiveType.declaration != nullptr);

	return (stream << *primitiveType.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::Action &action)
{
	return (stream << colorlog::String(action.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::PredicateDeclaration &predicateDeclaration)
{
	return (stream << colorlog::String(predicateDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::Predicate &predicate)
{
	return (stream << *predicate.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::DerivedPredicateDeclaration &derivedPredicateDeclaration)
{
	return (stream << colorlog::String(derivedPredicateDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const ::pddl::normalizedAST::DerivedPredicate &derivedPredicate)
{
	return (stream << *derivedPredicate.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
