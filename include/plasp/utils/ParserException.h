#ifndef __PLASP__UTILS__PARSER_EXCEPTION_H
#define __PLASP__UTILS__PARSER_EXCEPTION_H

#include <exception>
#include <string>

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
		explicit ParserException(size_t row, size_t column)
		:	ParserException(row, column, "Unspecified parser error")
		{
		}

		explicit ParserException(size_t row, size_t column, const char *message)
		:	ParserException(row, column, static_cast<std::string>(message))
		{
		}

		explicit ParserException(size_t row, size_t column, const std::string &message)
		:	m_message{std::to_string(row) + ":" + std::to_string(column) + "\t" + message}
		{
		}

		~ParserException() throw()
		{
		}

		const char *what() const throw()
		{
			if (m_message.empty())
				return "Unspecified parser error";

			return m_message.c_str();
		}

	private:
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
