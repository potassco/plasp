#include <plasp/utils/Logger.h>

#include <plasp/utils/Formatting.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Logger
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger()
:	m_outputStream(StandardStream::Out),
	m_errorStream(StandardStream::Err),
	m_warningLevel{Logger::WarningLevel::Normal}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(const Logger &other)
:	m_outputStream{other.m_outputStream},
	m_errorStream{other.m_errorStream},
	m_warningLevel{other.m_warningLevel}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger &Logger::operator=(const Logger &other)
{
	m_outputStream = other.m_outputStream;
	m_errorStream = other.m_errorStream;
	m_warningLevel = other.m_warningLevel;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(Logger &&other)
:	m_outputStream{std::move(other.m_outputStream)},
	m_errorStream{std::move(other.m_errorStream)},
	m_warningLevel{other.m_warningLevel}
{
	other.m_warningLevel = WarningLevel::Normal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger &Logger::operator=(Logger &&other)
{
	m_outputStream = std::move(other.m_outputStream);
	m_errorStream = std::move(other.m_errorStream);
	m_warningLevel = other.m_warningLevel;

	other.m_warningLevel = WarningLevel::Normal;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &Logger::outputStream()
{
	return m_outputStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &Logger::errorStream()
{
	return m_errorStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setWarningLevel(WarningLevel warningLevel)
{
	m_warningLevel = warningLevel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setColorPolicy(LogStream::ColorPolicy colorPolicy)
{
	m_outputStream.setColorPolicy(colorPolicy);
	m_errorStream.setColorPolicy(colorPolicy);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::logError(const std::string &message)
{
	m_errorStream
		<< Format(Color::Red, FontWeight::Bold) << "error:"
		<< ResetFormat() << " "
		<< Format(Color::White, FontWeight::Bold) << message
		<< ResetFormat() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::logError(const StreamCoordinate &coordinate, const std::string &message)
{
	m_errorStream
		<< Format(Color::White, FontWeight::Bold) << coordinate.sectionName << ":"
		<< std::to_string(coordinate.row) + ":" + std::to_string(coordinate.column) << ":"
		<< ResetFormat() << " "
		<< Format(Color::Red, FontWeight::Bold) << "error:"
		<< ResetFormat() << " "
		<< Format(Color::White, FontWeight::Bold) << message
		<< ResetFormat() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::logWarning(const StreamCoordinate &coordinate, const std::string &message)
{
	if (m_warningLevel == WarningLevel::Ignore)
		return;

	if (m_warningLevel == WarningLevel::Error)
		throw ParserException(coordinate, message);

	m_errorStream
		<< Format(Color::White, FontWeight::Bold) << coordinate.sectionName << ":"
		<< std::to_string(coordinate.row) + ":" + std::to_string(coordinate.column) << ":"
		<< ResetFormat() << " "
		<< Format(Color::Magenta, FontWeight::Bold) << "warning:"
		<< ResetFormat() << " "
		<< Format(Color::White, FontWeight::Bold) << message
		<< ResetFormat() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
