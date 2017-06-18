#ifndef __TOKENIZE__TOKENIZER_EXCEPTION_H
#define __TOKENIZE__TOKENIZER_EXCEPTION_H

#include <exception>
#include <string>

#include <tokenize/Location.h>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TokenizerException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class TokenizerException: public std::exception
{
	public:
		explicit TokenizerException(const Location &location)
		:	TokenizerException(location, "unspecified tokenizer error")
		{
		}

		explicit TokenizerException(const Location &location, const char *message)
		:	TokenizerException(location, static_cast<std::string>(message))
		{
		}

		explicit TokenizerException(const Location &location, const std::string &message)
		:	m_location{location},
			m_message{message}
		{
		}

		~TokenizerException() noexcept = default;

		const char *what() const noexcept
		{
			return m_message.c_str();
		}

		const Location &location() const
		{
			return m_location;
		}

		const std::string &message() const
		{
			return m_message;
		}

	private:
		Location m_location;
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
