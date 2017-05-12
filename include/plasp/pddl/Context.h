#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <plasp/output/Logger.h>
#include <plasp/pddl/Tokenizer.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Context
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context
{
	public:
		Context() = default;
		~Context() = default;

		explicit Context(Tokenizer &&otherTokenizer, output::Logger &otherLogger)
		:	tokenizer{std::move(otherTokenizer)},
			logger(otherLogger)
		{
		}

		Context(const Context &other) = delete;
		Context &operator=(const Context &other) = delete;

		Context(Context &&other)
		:	tokenizer(std::move(other.tokenizer)),
			logger(other.logger)
		{
		}

		Context &operator=(Context &&other) = delete;

		constexpr static const char *auxiliaryPrefix()
		{
			return "__plasp_";
		}

		Tokenizer tokenizer;
		output::Logger &logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
