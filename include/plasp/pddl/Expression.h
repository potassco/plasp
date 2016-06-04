#ifndef __PLASP__PDDL__EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION_H

#include <memory>

#include <boost/variant.hpp>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Expression
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;
class ExpressionVisitor;

class Expression;
using ExpressionPointer = std::unique_ptr<Expression>;
using Expressions = std::vector<ExpressionPointer>;

namespace expressions
{
class And;
using AndPointer = std::unique_ptr<And>;

class Constant;
using ConstantPointer = std::unique_ptr<Constant>;
using Constants = std::vector<ConstantPointer>;

class Either;
using EitherPointer = std::unique_ptr<Either>;

class Not;
using NotPointer = std::unique_ptr<Not>;

class Or;
using OrPointer = std::unique_ptr<Or>;

class Predicate;
using PredicatePointer = std::unique_ptr<Predicate>;
using Predicates = std::vector<PredicatePointer>;

class PredicateDeclaration;
using PredicateDeclarationPointer = std::unique_ptr<PredicateDeclaration>;
using PredicateDeclarations = std::vector<PredicateDeclarationPointer>;

class PrimitiveType;
using PrimitiveTypePointer = std::unique_ptr<PrimitiveType>;
using PrimitiveTypes = std::vector<PrimitiveTypePointer>;

template<class Type>
class Reference;
template<class Type>
using ReferencePointer = std::unique_ptr<Reference<Type>>;

class Variable;
using VariablePointer = std::unique_ptr<Variable>;
using Variables = std::vector<VariablePointer>;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class Expression
{
	public:
		virtual void accept(ExpressionVisitor &expressionVisitor) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(Context &context,
	const expressions::Variables &parameters);
ExpressionPointer parseExpression(Context &context, const expressions::Variables &parameters);

ExpressionPointer parseEffectExpression(Context &context, const expressions::Variables &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
