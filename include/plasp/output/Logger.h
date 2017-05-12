#ifndef __PLASP__OUTPUT__LOGGER_H
#define __PLASP__OUTPUT__LOGGER_H

#include <string>

#include <plasp/output/ColorStream.h>
#include <plasp/output/Priority.h>

#include <tokenize/Location.h>

namespace plasp
{
namespace output
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Logger
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Logger
{
	public:
		explicit Logger();
		explicit Logger(ColorStream &&outputStream);
		explicit Logger(ColorStream &&outputStream, ColorStream &&errorStream);

		ColorStream &outputStream();
		ColorStream &errorStream();

		// The priority from which on messages should be printed
		void setLogPriority(Priority logPriority);
		// Messages with this priority (or higher) will terminate the program’s execution
		void setAbortPriority(Priority abortPriority);
		void setColorPolicy(ColorStream::ColorPolicy colorPolicy);

		void log(Priority priority, const char *message);
		void log(Priority priority, const std::string &message);
		void log(Priority priority, const tokenize::Location &location, const char *message);
		void log(Priority priority, const tokenize::Location &location, const std::string &message);

	private:
		ColorStream m_outputStream;
		ColorStream m_errorStream;

		Priority m_logPriority;
		Priority m_abortPriority;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
