#ifndef __PLASP__UTILS__PARSER_EXCEPTION_H
#define __PLASP__UTILS__PARSER_EXCEPTION_H

#include <exception>
#include <string>

#include <plasp/utils/Parser.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserException: public std::exception
{
	public:
		explicit ParserException(const utils::Parser &parser)
		:	ParserException(parser, "Unspecified parser error")
		{
		}

		explicit ParserException(const utils::Parser &parser, const char *message)
		:	ParserException(parser, static_cast<std::string>(message))
		{
		}

		explicit ParserException(const utils::Parser &parser, const std::string &message)
		:	m_coordinate{parser.coordinate()},
			m_message{message},
			m_plainMessage{m_coordinate.sectionName + ":" + std::to_string(m_coordinate.row)
				+ ":" + std::to_string(m_coordinate.column) + " " + m_message}
		{
		}

		~ParserException() throw()
		{
		}

		const char *what() const throw()
		{
			return m_plainMessage.c_str();
		}

		const Parser::Coordinate &coordinate() const
		{
			return m_coordinate;
		}

		const std::string &message() const
		{
			return m_message;
		}

	private:
		Parser::Coordinate m_coordinate;
		std::string m_message;
		std::string m_plainMessage;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
