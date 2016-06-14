#ifndef __PLASP__UTILS__LOGGER_H
#define __PLASP__UTILS__LOGGER_H

#include <string>

#include <plasp/utils/Parser.h>
#include <plasp/utils/ParserException.h>

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

		void exception(const std::string &errorType, const std::string &message);
		void parserException(const Parser::Coordinate &coordinate, const std::string &message);
		void parserWarning(const Parser &parser, const std::string &message);

	private:
		WarningLevel m_warningLevel;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
