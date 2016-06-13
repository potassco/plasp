#ifndef __PLASP__UTILS__LOGGER_H
#define __PLASP__UTILS__LOGGER_H

#include <string>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Logger
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Logger
{
	public:
		enum class WarningLevel
		{
			Normal,
			Error,
			Ignore
		};

	public:
		Logger();

		Logger(const Logger &other);
		Logger &operator=(const Logger &other);

		Logger(Logger &&other);
		Logger &operator=(Logger &&other);

		void setWarningLevel(WarningLevel warningLevel);

		void parserWarning(const Parser &parser, const std::string &message);

	private:
		WarningLevel m_warningLevel;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
