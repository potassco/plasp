#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

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

		explicit Context(utils::Parser &&parser)
		:	parser{std::move(parser)}
		{
		}

		explicit Context(utils::Logger &&logger)
		:	logger{std::move(logger)}
		{
		}

		explicit Context(utils::Parser &&parser, utils::Logger &&logger)
		:	parser{std::move(parser)},
			logger{std::move(logger)}
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

		utils::Parser parser;
		utils::Logger logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
