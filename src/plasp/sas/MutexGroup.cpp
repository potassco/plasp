#include <plasp/sas/MutexGroup.h>

#include <iostream>

#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MutexGroup
//
////////////////////////////////////////////////////////////////////////////////////////////////////

MutexGroup MutexGroup::fromSAS(utils::Parser &parser, const Variables &variables)
{
	MutexGroup mutexGroup;

	parser.expect<std::string>("begin_mutex_group");

	const auto numberOfFacts = parser.parse<size_t>();
	mutexGroup.m_facts.reserve(numberOfFacts);

	for (size_t j = 0; j < numberOfFacts; j++)
	{
		mutexGroup.m_facts.emplace_back(Fact::fromSAS(parser, variables));

		if (mutexGroup.m_facts[j].value() == Value::None)
			throw utils::ParserException(parser, "Mutex groups must not contain <none of those> values");
	}

	parser.expect<std::string>("end_mutex_group");

	return mutexGroup;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const MutexGroup::Facts &MutexGroup::facts() const
{
	return m_facts;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
