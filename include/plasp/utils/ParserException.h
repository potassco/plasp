#ifndef __UTILS__PARSER_EXCEPTION_H
#define __UTILS__PARSER_EXCEPTION_H

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
		explicit ParserException()
		{
		}

		explicit ParserException(const char *message)
		:	m_message(message)
		{
		}

		explicit ParserException(const std::string &message)
		:	m_message(message)
		{
		}

		~ParserException() throw()
		{
		}

		const char *what() const throw()
		{
			if (m_message.empty())
				return "Unspecified error while parsing SAS description file";

			return m_message.c_str();
		}

	private:
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
