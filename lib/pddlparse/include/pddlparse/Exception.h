#ifndef __PDDL_PARSE__EXCEPTION_H
#define __PDDL_PARSE__EXCEPTION_H

#include <exception>
#include <string>

#include <tokenize/Stream.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Exception
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Exception: public std::exception
{
	public:
		explicit Exception()
		:	Exception("unspecified parser error")
		{
		}

		explicit Exception(const char *message)
		:	Exception(static_cast<std::string>(message))
		{
		}

		explicit Exception(const std::string &message)
		:	m_message{message}
		{
		}

		explicit Exception(const tokenize::Location &location)
		:	Exception(location, "unspecified parser error")
		{
		}

		explicit Exception(const tokenize::Location &location, const char *message)
		:	Exception(location, static_cast<std::string>(message))
		{
		}

		explicit Exception(const tokenize::Location &location, const std::string &message)
		:	m_location{location},
			m_message{message},
			// TODO: refactor
			m_plainMessage{std::string(m_location.sectionStart) + ":" + std::to_string(m_location.rowStart)
				+ ":" + std::to_string(m_location.columnStart) + " " + m_message}
		{
		}

		~Exception() noexcept = default;

		const char *what() const throw()
		{
			return m_plainMessage.c_str();
		}

		const tokenize::Location &location() const
		{
			return m_location;
		}

		const std::string &message() const
		{
			return m_message;
		}

	private:
		tokenize::Location m_location;
		std::string m_message;
		std::string m_plainMessage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserException : public Exception
{
	public:
		using Exception::Exception;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
