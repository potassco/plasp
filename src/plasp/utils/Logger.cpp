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
:	m_warningLevel{Logger::WarningLevel::Normal}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(const Logger &other)
:	m_warningLevel{other.m_warningLevel}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger &Logger::operator=(const Logger &other)
{
	m_warningLevel = other.m_warningLevel;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger::Logger(Logger &&other)
:	m_warningLevel{other.m_warningLevel}
{
	other.m_warningLevel = WarningLevel::Normal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Logger &Logger::operator=(Logger &&other)
{
	m_warningLevel = other.m_warningLevel;
	other.m_warningLevel = WarningLevel::Normal;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream<StandardStream::Out> &Logger::outputStream()
{
	return m_outputStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream<StandardStream::Err> &Logger::errorStream()
{
	return m_errorStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Logger::setWarningLevel(WarningLevel warningLevel)
{
	m_warningLevel = warningLevel;
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

void Logger::logError(const Parser::Coordinate &coordinate, const std::string &message)
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

void Logger::logWarning(const Parser &parser, const std::string &message)
{
	if (m_warningLevel == WarningLevel::Ignore)
		return;

	if (m_warningLevel == WarningLevel::Error)
		throw ParserException(parser, message);

	const auto coordinate = parser.coordinate();

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
