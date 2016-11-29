#include <plasp/output/Logger.h>

#include <plasp/output/Formatting.h>

namespace plasp
{
namespace output
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
	m_logPriority{Priority::Warning}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(Logger &&other)
:	m_outputStream{std::move(other.m_outputStream)},
	m_errorStream{std::move(other.m_errorStream)},
	m_logPriority{other.m_logPriority}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger &Logger::operator=(Logger &&other)
{
	*this = std::move(other);

	return *this;
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

void Logger::setColorPolicy(ColorStream::ColorPolicy colorPolicy)
{
	m_outputStream.setColorPolicy(colorPolicy);
	m_errorStream.setColorPolicy(colorPolicy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const char *message)
{
	const auto priorityID = static_cast<int>(priority);

	if (priorityID < static_cast<int>(m_logPriority))
		return;

	m_errorStream
		<< priorityFormat(priority) << priorityName(priority) << ":"
		<< ResetFormat() << " "
		<< MessageBodyFormat << message
		<< ResetFormat() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const std::string &message)
{
	log(priority, message.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const input::Location &location, const char *message)
{
	const auto priorityID = static_cast<int>(priority);

	if (priorityID < static_cast<int>(m_logPriority))
		return;

	m_errorStream
		<< LocationFormat
		<< location.sectionStart << ":" << location.rowStart << ":" << location.columnStart << ":"
		<< ResetFormat() << " "
		<< priorityFormat(priority) << priorityName(priority) << ":"
		<< ResetFormat() << " "
		<< MessageBodyFormat << message
		<< ResetFormat() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::log(Priority priority, const input::Location &location, const std::string &message)
{
	log(priority, location, message.c_str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
