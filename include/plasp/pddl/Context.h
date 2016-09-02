#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <plasp/pddl/Parser.h>
#include <plasp/utils/Logger.h>

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

		explicit Context(Parser &&otherParser)
		:	parser{std::move(otherParser)}
		{
		}

		explicit Context(utils::Logger &&otherLogger)
		:	logger{std::move(otherLogger)}
		{
		}

		explicit Context(Parser &&otherParser, utils::Logger &&otherLogger)
		:	parser{std::move(otherParser)},
			logger{std::move(otherLogger)}
		{
		}

		Context(const Context &other) = delete;
		Context &operator=(const Context &other) = delete;

		Context(Context &&other)
		:	parser(std::move(other.parser)),
			logger(std::move(other.logger))
		{
		}

		Context &operator=(Context &&other)
		{
			parser = std::move(other.parser);
			logger = std::move(other.logger);

			return *this;
		}

		constexpr static const char *auxiliaryPrefix()
		{
			return "__plasp_";
		}

		Parser parser;
		utils::Logger logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
