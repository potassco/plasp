#include <plasp/pddl/expressions/Not.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Not
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Not::Not()
:	m_argument{nullptr}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Not::setArgument(const Expression *argument)
{
	m_argumentStorage = nullptr;
	m_argument = argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Not::setArgument(ExpressionPointer &&argument)
{
	m_argumentStorage = std::move(argument);
	m_argument = m_argumentStorage.get();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Expression *Not::argument() const
{
	return m_argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ExpressionPointer Not::normalize()
{
	BOOST_ASSERT(m_argumentStorage);

	// Remove double negations immediately
	if (m_argumentStorage->expressionType() == Expression::Type::Not)
	{
		auto &argument = dynamic_cast<Not &>(*m_argumentStorage);

		auto normalized = std::move(argument.m_argumentStorage);
		auto normalizedInner = normalized->normalize();

		if (normalizedInner)
			return normalizedInner;

		return normalized;
	}

	auto normalizedArgument = m_argumentStorage->normalize();

	// Replace argument if changed by normalization
	if (normalizedArgument)
		setArgument(std::move(normalizedArgument));

	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}
