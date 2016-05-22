#include <plasp/sas/MutexGroup.h>

#include <iostream>

#include <plasp/utils/Parsing.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MutexGroup
//
////////////////////////////////////////////////////////////////////////////////////////////////////

MutexGroup MutexGroup::fromSAS(std::istream &istream, const Variables &variables)
{
	MutexGroup mutexGroup;

	utils::parseExpected<std::string>(istream, "begin_mutex_group");

	const auto numberOfFacts = utils::parse<size_t>(istream);
	mutexGroup.m_facts.reserve(numberOfFacts);

	for (size_t j = 0; j < numberOfFacts; j++)
	{
		mutexGroup.m_facts.emplace_back(Fact::fromSAS(istream, variables));
	}

	utils::parseExpected<std::string>(istream, "end_mutex_group");

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
