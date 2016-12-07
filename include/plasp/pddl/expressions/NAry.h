#ifndef __PLASP__PDDL__EXPRESSIONS__N_ARY_H
#define __PLASP__PDDL__EXPRESSIONS__N_ARY_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>

#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>

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
		ExpressionPointer copy() override;

		void setArgument(size_t i, ExpressionPointer argument);
		void addArgument(ExpressionPointer argument);
		Expressions &arguments();
		const Expressions &arguments() const;

		ExpressionPointer reduced() override;
		ExpressionPointer existentiallyQuantified() override;
		ExpressionPointer simplified() override;

		void collectParameters(std::set<VariablePointer> &parameters) override;

		void print(std::ostream &ostream) const override;

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

	// Parse arguments of the expression
	while (parser.currentCharacter() != ')')
	{
		expression->addArgument(parseExpression(context, expressionContext));

		parser.skipWhiteSpace();
	}

	if (expression->m_arguments.empty())
		context.logger.log(output::Priority::Warning, parser.location(), "“" + Derived::Identifier + "” expressions should not be empty");

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
ExpressionPointer NAry<Derived>::copy()
{
	auto result = new Derived;

	result->m_arguments.resize(m_arguments.size());

	for (size_t i = 0; i < m_arguments.size(); i++)
		result->m_arguments[i] = m_arguments[i]->copy();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
void NAry<Derived>::setArgument(size_t i, ExpressionPointer expression)
{
	BOOST_ASSERT_MSG(i <= m_arguments.size(), "Index out of range");

	m_arguments[i] = expression;
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
Expressions &NAry<Derived>::arguments()
{
	return m_arguments;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer NAry<Derived>::reduced()
{
	for (auto &argument : m_arguments)
	{
		BOOST_ASSERT(argument);

		argument = argument->reduced();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer NAry<Derived>::existentiallyQuantified()
{
	for (auto &argument : m_arguments)
	{
		BOOST_ASSERT(argument);

		argument = argument->existentiallyQuantified();
	}

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer NAry<Derived>::simplified()
{
	// Associate same-type children, such as (a && (b && c)) == (a && b && c)
	for (size_t i = 0; i < m_arguments.size();)
	{
		auto &argument = m_arguments[i];
		argument = argument->simplified();

		if (argument->expressionType() != Derived::ExpressionType)
		{
			i++;
			continue;
		}

		auto &nAryExpression = argument->template as<Derived>();

		BOOST_ASSERT(!nAryExpression.arguments().empty());

		// Remove former child by replacing it with the first one of the child
		m_arguments[i] = nAryExpression.arguments().front();

		// Reserve space for new arguments
		m_arguments.reserve(m_arguments.size() + nAryExpression.arguments().size() - 1);

		// Copy all but first element
		m_arguments.insert(m_arguments.end(), nAryExpression.arguments().begin() + 1, nAryExpression.arguments().end());
	}

	// TODO: recognize tautologies
	// TODO: introduce/handle boolean values

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline void NAry<Derived>::collectParameters(std::set<VariablePointer> &parameters)
{
	for (const auto &argument : m_arguments)
		argument->collectParameters(parameters);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline void NAry<Derived>::print(std::ostream &ostream) const
{
	ostream << "(" << Derived::Identifier;

	for (const auto &argument : m_arguments)
	{
		ostream << " ";
		argument->print(ostream);
	}

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
