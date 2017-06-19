#ifndef __PLASP__PDDL__TRANSLATION__PRIMITIVES_H
#define __PLASP__PDDL__TRANSLATION__PRIMITIVES_H

#include <pddlparse/AST.h>

#include <plasp/output/Formatting.h>
#include <plasp/output/TranslatorException.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Primitives
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::ConstantDeclaration &constantDeclaration)
{
	assert(!constantDeclaration.name.empty());

	return (stream << output::String(constantDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::Constant &constant)
{
	assert(constant.declaration != nullptr);

	return (stream << *constant.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::PrimitiveTypeDeclaration &primitiveTypeDeclaration)
{
	assert(!primitiveTypeDeclaration.name.empty());

	return (stream << output::String(primitiveTypeDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::PrimitiveType &primitiveType)
{
	assert(primitiveType.declaration != nullptr);

	return (stream << *primitiveType.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, ::pddl::ast::VariableDeclaration &variableDeclaration)
{
	assert(!variableDeclaration.name.empty());
	assert(std::isalpha(variableDeclaration.name[0]));

	if (!std::isupper(variableDeclaration.name[0]))
		variableDeclaration.name[0] = std::toupper(variableDeclaration.name[0]);

	return (stream
		<< output::Format({output::Color::Green, output::FontWeight::Bold})
		<< variableDeclaration.name
		<< output::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline output::ColorStream &operator<<(output::ColorStream &stream, ::pddl::ast::Variable &variable)
{
	assert(variable.declaration != nullptr);

	return (stream << *variable.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::Action &action)
{
	return (stream << output::String(action.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::PredicateDeclaration &predicateDeclaration)
{
	return (stream << output::String(predicateDeclaration.name.c_str()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: move to appropriate header
inline output::ColorStream &operator<<(output::ColorStream &stream, const ::pddl::ast::Predicate &predicate)
{
	return (stream << *predicate.declaration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
