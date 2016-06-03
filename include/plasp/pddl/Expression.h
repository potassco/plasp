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
class AndExpression;
using AndExpressionPointer = std::unique_ptr<AndExpression>;

class NotExpression;
using NotExpressionPointer = std::unique_ptr<NotExpression>;

class OrExpression;
using OrExpressionPointer = std::unique_ptr<OrExpression>;

class PredicateExpression;
using PredicateExpressionPointer = std::unique_ptr<PredicateExpression>;

class VariableExpression;
using VariableExpressionPointer = std::unique_ptr<VariableExpression>;
using VariableExpressions = std::vector<VariableExpressionPointer>;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

class Expression
{
	public:
		virtual void accept(ExpressionVisitor &expressionVisitor) const = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer parsePreconditionExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters);
ExpressionPointer parseExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters);

ExpressionPointer parseEffectExpression(utils::Parser &parser, Context &context,
	const expressions::VariableExpressions &parameters);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
