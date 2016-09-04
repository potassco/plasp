#ifndef __PLASP__PDDL__EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION_H

#include <memory>

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
class Domain;
class ExpressionContext;
class ExpressionVisitor;
class Problem;

class Expression;
using ExpressionPointer = std::unique_ptr<Expression>;
using Expressions = std::vector<ExpressionPointer>;

namespace expressions
{
class And;
using AndPointer = std::unique_ptr<And>;

class At;
using AtPointer = std::unique_ptr<At>;

class Constant;
using ConstantPointer = std::unique_ptr<Constant>;
using Constants = std::vector<ConstantPointer>;

class Either;
using EitherPointer = std::unique_ptr<Either>;

class Imply;
using ImplyPointer = std::unique_ptr<Imply>;

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

class Unsupported;
using UnsupportedPointer = std::unique_ptr<Unsupported>;

class Variable;
using VariablePointer = std::unique_ptr<Variable>;
using Variables = std::vector<VariablePointer>;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class Expression
{
	public:
		enum class Type
		{
			And,
			At,
			Binary,
			Constant,
			Dummy,
			Either,
			Imply,
			Not,
			Or,
			PredicateDeclaration,
			Predicate,
			PrimitiveType,
			Reference,
			Unsupported,
			Variable,
		};

	public:
		virtual ~Expression() = default;

		virtual Type expressionType() const = 0;

		// Normalizes the expression dependent on its type, recursing through all child expressions
		//
		// Result:
		//     * a new expression pointer to replace this one if required; this object is then empty
		//     * nullptr otherwise; the object may or may not have changed
		virtual ExpressionPointer normalize() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class ExpressionCRTP: public Expression
{
	public:
		Type expressionType() const override final
		{
			return Derived::ExpressionType;
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parseLiteral(Context &context, ExpressionContext &expressionContext);
ExpressionPointer parseAtomicFormula(Context &context, ExpressionContext &expressionContext);

ExpressionPointer parsePreconditionExpression(Context &context,
	ExpressionContext &expressionContext);
ExpressionPointer parseExpression(Context &context, ExpressionContext &expressionContext);

ExpressionPointer parseEffectExpression(Context &context,
	ExpressionContext &expressionContext);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
