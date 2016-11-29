#ifndef __PLASP__UTILS__LOGGER_H
#define __PLASP__UTILS__LOGGER_H

#include <string>

#include <plasp/utils/LogStream.h>
#include <plasp/utils/ParserException.h>
#include <plasp/utils/StreamCoordinate.h>

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
			Show,
			Error,
			Ignore
		};

	public:
		Logger();

		Logger(const Logger &other);
		Logger &operator=(const Logger &other);

		Logger(Logger &&other);
		Logger &operator=(Logger &&other);

		LogStream &outputStream();
		LogStream &errorStream();

		void setWarningLevel(WarningLevel warningLevel);
		void setColorPolicy(LogStream::ColorPolicy colorPolicy);

		void logError(const std::string &message);
		void logError(const StreamCoordinate &coordinate, const std::string &message);
		void logWarning(const StreamCoordinate &parserCoordinate, const std::string &message);

	private:
		LogStream m_outputStream;
		LogStream m_errorStream;

		WarningLevel m_warningLevel;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
