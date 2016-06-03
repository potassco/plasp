#ifndef __PLASP__PDDL__EXPRESSION_H
#define __PLASP__PDDL__EXPRESSION_H

#include <memory>

#include <boost/variant.hpp>

#include <plasp/pddl/Variable.h>
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

class Not;
using NotPointer = std::unique_ptr<Not>;

class Or;
using OrPointer = std::unique_ptr<Or>;

class Predicate;
using PredicatePointer = std::unique_ptr<Predicate>;

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

ExpressionPointer parsePreconditionExpression(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters);
ExpressionPointer parseExpression(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters);

ExpressionPointer parseEffectExpression(utils::Parser &parser, Context &context,
	const expressions::Variables &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
