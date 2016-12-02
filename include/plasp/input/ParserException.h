#ifndef __PLASP__INPUT__PARSER_EXCEPTION_H
#define __PLASP__INPUT__PARSER_EXCEPTION_H

#include <exception>
#include <string>

#include <plasp/input/Location.h>

namespace plasp
{
namespace input
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserException: public std::exception
{
	public:
		explicit ParserException(const input::Location &location)
		:	ParserException(location, "unspecified parser error")
		{
		}

		explicit ParserException(const input::Location &location, const char *message)
		:	ParserException(location, static_cast<std::string>(message))
		{
		}

		explicit ParserException(const input::Location &location, const std::string &message)
		:	m_location{location},
			m_message{message},
			// TODO: refactor
			m_plainMessage{std::string(m_location.sectionStart) + ":" + std::to_string(m_location.rowStart)
				+ ":" + std::to_string(m_location.columnStart) + " " + m_message}
		{
		}

		~ParserException() throw()
		{
		}

		const char *what() const throw()
		{
			return m_plainMessage.c_str();
		}

		const input::Location &location() const
		{
			return m_location;
		}

		const std::string &message() const
		{
			return m_message;
		}

	private:
		input::Location m_location;
		std::string m_message;
		std::string m_plainMessage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
