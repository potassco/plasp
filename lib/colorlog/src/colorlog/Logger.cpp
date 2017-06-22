#include <colorlog/Logger.h>

#include <colorlog/Formatting.h>

namespace colorlog
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Logger
//
////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr Format priorityFormat(Priority priority)
{
	switch (priority)
	{
		case Priority::Debug:
			return {Color::Green, FontWeight::Bold};
		case Priority::Info:
			return {Color::Blue, FontWeight::Bold};
		case Priority::Warning:
			return {Color::Magenta, FontWeight::Bold};
		case Priority::Error:
			return {Color::Red, FontWeight::Bold};
	}

	return {Color::White, FontWeight::Bold};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr const Format MessageBodyFormat = {Color::White, FontWeight::Bold};

////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr const Format LocationFormat = {Color::White, FontWeight::Bold};

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger()
:	Logger(std::cout, std::cerr)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(ColorStream &&outputStream)
:	Logger(std::forward<ColorStream &&>(outputStream), std::cerr)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(ColorStream &&outputStream, ColorStream &&errorStream)
:	m_outputStream{outputStream},
	m_errorStream{errorStream},
	m_logPriority{Priority::Info},
	m_abortPriority{Priority::Error}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &Logger::outputStream()
{
	return m_outputStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &Logger::errorStream()
{
	return m_errorStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setLogPriority(Priority logPriority)
{
	m_logPriority = logPriority;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setAbortPriority(Priority abortPriority)
{
	m_abortPriority = abortPriority;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setColorPolicy(ColorStream::ColorPolicy colorPolicy)
{
	m_outputStream.setColorPolicy(colorPolicy);
	m_errorStream.setColorPolicy(colorPolicy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const char *message)
{
	const auto priorityID = static_cast<int>(priority);

	if (priorityID < static_cast<int>(m_logPriority) &&
		priorityID < static_cast<int>(m_abortPriority))
	{
		return;
	}

	m_errorStream
		<< priorityFormat(priority) << priorityName(priority) << ":"
		<< ResetFormat() << " "
		<< MessageBodyFormat << message
		<< ResetFormat() << std::endl;

	if (priority != Priority::Error && priorityID >= static_cast<int>(m_abortPriority))
		throw std::runtime_error("received warning (treated as error by configuration)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const std::string &message)
{
	log(priority, message.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const tokenize::Location &location, const char *message)
{
	const auto priorityID = static_cast<int>(priority);

	// Always show messages that lead to program termination
	if (priorityID < static_cast<int>(m_logPriority) &&
	    priorityID < static_cast<int>(m_abortPriority))
	{
		return;
	}

	m_errorStream
		<< LocationFormat
		<< location.sectionStart << ":" << location.rowStart << ":" << location.columnStart << ":"
		<< ResetFormat() << " "
		<< priorityFormat(priority) << priorityName(priority) << ":"
		<< ResetFormat() << " "
		<< MessageBodyFormat << message
		<< ResetFormat() << std::endl;

	// TODO: print original warning message
	// TODO: refactor
	if (priority != Priority::Error && priorityID >= static_cast<int>(m_abortPriority))
		throw std::runtime_error("received warning (treated as error by configuration)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const tokenize::Location &location, const std::string &message)
{
	log(priority, location, message.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
