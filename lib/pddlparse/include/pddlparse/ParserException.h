#ifndef __PDDL_PARSE__PARSER_EXCEPTION_H
#define __PDDL_PARSE__PARSER_EXCEPTION_H

#include <exception>
#include <string>

#include <tokenize/Stream.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserException: public std::exception
{
	public:
		explicit ParserException()
		:	ParserException("unspecified parser error")
		{
		}

		explicit ParserException(const char *message)
		:	ParserException(static_cast<std::string>(message))
		{
		}

		explicit ParserException(const std::string &message)
		:	m_message{message}
		{
		}

		explicit ParserException(const tokenize::Location &location)
		:	ParserException(location, "unspecified parser error")
		{
		}

		explicit ParserException(const tokenize::Location &location, const char *message)
		:	ParserException(location, static_cast<std::string>(message))
		{
		}

		explicit ParserException(const tokenize::Location &location, const std::string &message)
		:	m_location{location},
			m_message{message},
			// TODO: refactor
			m_plainMessage{std::string(m_location.sectionStart) + ":" + std::to_string(m_location.rowStart)
				+ ":" + std::to_string(m_location.columnStart) + " " + m_message}
		{
		}

		~ParserException() noexcept = default;

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

}

#endif
