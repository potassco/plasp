#ifndef __PLASP__TRANSLATOR_EXCEPTION_H
#define __PLASP__TRANSLATOR_EXCEPTION_H

#include <exception>
#include <string>

namespace plasp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class TranslatorException: public std::exception
{
	public:
		explicit TranslatorException()
		{
		}

		explicit TranslatorException(const char *message)
		:	m_message(message)
		{
		}

		explicit TranslatorException(const std::string &message)
		:	m_message(message)
		{
		}

		~TranslatorException() throw()
		{
		}

		const char *what() const throw()
		{
			if (m_message.empty())
				return "Unspecified error while translating description";

			return m_message.c_str();
		}

	private:
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
