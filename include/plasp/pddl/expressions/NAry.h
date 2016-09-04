#ifndef __PLASP__PDDL__EXPRESSIONS__N_ARY_H
#define __PLASP__PDDL__EXPRESSIONS__N_ARY_H

#include <plasp/pddl/ConsistencyException.h>
#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NAry
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class NAry: public ExpressionCRTP<Derived>
{
	public:
		template<typename ExpressionParser>
		static boost::intrusive_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		void addArgument(ExpressionPointer argument);
		const Expressions &arguments() const;

		ExpressionPointer normalize() override;

	protected:
		Expressions m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
boost::intrusive_ptr<Derived> NAry<Derived>::parse(Context &context,
	ExpressionContext &expressionContext, ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip(Derived::Identifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = boost::intrusive_ptr<Derived>(new Derived);

	parser.skipWhiteSpace();

	// Assume that expression identifier (and, or, etc.) is already parsed
	// Parse arguments of the expression
	while (parser.currentCharacter() != ')')
	{
		expression->addArgument(parseExpression(context, expressionContext));

		parser.skipWhiteSpace();
	}

	if (expression->m_arguments.empty())
		context.logger.logWarning(parser.coordinate(), "“" + Derived::Identifier + "” expressions should not be empty");

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
void NAry<Derived>::addArgument(ExpressionPointer argument)
{
	if (!argument)
		return;

	m_arguments.emplace_back(argument);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const Expressions &NAry<Derived>::arguments() const
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer NAry<Derived>::normalize()
{
	for (size_t i = 0; i < m_arguments.size(); i++)
	{
		BOOST_ASSERT(m_arguments[i]);

		auto normalizedArgument = m_arguments[i]->normalize();

		// Replace argument if changed by normalization
		if (!normalizedArgument)
			continue;

		m_arguments[i] = std::move(normalizedArgument);
	}

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
