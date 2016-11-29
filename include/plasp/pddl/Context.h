#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <plasp/output/Logger.h>
#include <plasp/pddl/Parser.h>

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

		explicit Context(Parser &&otherParser, output::Logger &otherLogger)
		:	parser{std::move(otherParser)},
			logger(otherLogger)
		{
		}

		Context(const Context &other) = delete;
		Context &operator=(const Context &other) = delete;

		Context(Context &&other)
		:	parser(std::move(other.parser)),
			logger(other.logger)
		{
		}

		Context &operator=(Context &&other) = delete;

		constexpr static const char *auxiliaryPrefix()
		{
			return "__plasp_";
		}

		Parser parser;
		output::Logger &logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
