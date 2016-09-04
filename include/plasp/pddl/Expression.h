#ifndef __PLASP__PDDL__EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION_H

#include <iosfwd>

#include <boost/intrusive_ptr.hpp>

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
using ExpressionPointer = boost::intrusive_ptr<Expression>;
using Expressions = std::vector<ExpressionPointer>;

namespace expressions
{
class And;
using AndPointer = boost::intrusive_ptr<And>;

class At;
using AtPointer = boost::intrusive_ptr<At>;

class Constant;
using ConstantPointer = boost::intrusive_ptr<Constant>;
using Constants = std::vector<ConstantPointer>;

class Dummy;
using DummyPointer = boost::intrusive_ptr<Dummy>;

class Either;
using EitherPointer = boost::intrusive_ptr<Either>;

class Imply;
using ImplyPointer = boost::intrusive_ptr<Imply>;

class Not;
using NotPointer = boost::intrusive_ptr<Not>;

class Or;
using OrPointer = boost::intrusive_ptr<Or>;

class Predicate;
using PredicatePointer = boost::intrusive_ptr<Predicate>;
using Predicates = std::vector<PredicatePointer>;

class PredicateDeclaration;
using PredicateDeclarationPointer = boost::intrusive_ptr<PredicateDeclaration>;
using PredicateDeclarations = std::vector<PredicateDeclarationPointer>;

class PrimitiveType;
using PrimitiveTypePointer = boost::intrusive_ptr<PrimitiveType>;
using PrimitiveTypes = std::vector<PrimitiveTypePointer>;

template<class Type>
class Reference;
template<class Type>
using ReferencePointer = boost::intrusive_ptr<Reference<Type>>;

class Unsupported;
using UnsupportedPointer = boost::intrusive_ptr<Unsupported>;

class Variable;
using VariablePointer = boost::intrusive_ptr<Variable>;
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

		ExpressionPointer normalized();
		virtual ExpressionPointer reduced();
		virtual ExpressionPointer negationNormalized();
		virtual ExpressionPointer simplified();
		ExpressionPointer negated();

		virtual void print(std::ostream &ostream) const = 0;

	private:
		friend void intrusive_ptr_add_ref(Expression *expression);
		friend void intrusive_ptr_release(Expression *expression);

		size_t m_referenceCount = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void intrusive_ptr_add_ref(Expression *expression)
{
    expression->m_referenceCount++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline void intrusive_ptr_release(Expression *expression)
{
    if (--expression->m_referenceCount == 0)
        delete expression;
}

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
