#include <plasp/sas/MutexGroup.h>

#include <iostream>

#include <tokenize/TokenizerException.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MutexGroup
//
////////////////////////////////////////////////////////////////////////////////////////////////////

MutexGroup MutexGroup::fromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables)
{
	MutexGroup mutexGroup;

	tokenizer.expect<std::string>("begin_mutex_group");

	const auto numberOfFacts = tokenizer.get<size_t>();
	mutexGroup.m_facts.reserve(numberOfFacts);

	for (size_t j = 0; j < numberOfFacts; j++)
	{
		mutexGroup.m_facts.emplace_back(Fact::fromSAS(tokenizer, variables));

		if (mutexGroup.m_facts[j].value() == Value::None)
			throw tokenize::TokenizerException(tokenizer.location(), "mutex groups must not contain <none of those> values");
	}

	tokenizer.expect<std::string>("end_mutex_group");

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
