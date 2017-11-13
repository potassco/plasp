#ifndef __PDDL__EXCEPTION_H
#define __PDDL__EXCEPTION_H

#include <exception>
#include <experimental/optional>
#include <string>

#include <tokenize/Location.h>

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
		Exception()
		:	Exception("unspecified parser error")
		{
		}

		Exception(const char *message)
		:	Exception(static_cast<std::string>(message))
		{
		}

		Exception(const std::string &message)
		:	m_message{message}
		{
		}

		Exception(tokenize::Location &&location)
		:	Exception(std::forward<tokenize::Location>(location), "unspecified parser error")
		{
		}

		Exception(tokenize::Location &&location, const char *message)
		:	Exception(std::forward<tokenize::Location>(location), static_cast<std::string>(message))
		{
		}

		Exception(tokenize::Location &&location, const std::string &message)
		:	m_location{std::move(location)},
			m_message{message}
		{
		}

		~Exception() noexcept = default;

		const char *what() const noexcept
		{
			return m_message.c_str();
		}

		const std::experimental::optional<tokenize::Location> &location() const
		{
			return m_location;
		}

		const std::string &message() const
		{
			return m_message;
		}

	private:
		std::experimental::optional<tokenize::Location> m_location;
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ParserException : public Exception
{
	public:
		using Exception::Exception;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class NormalizationException : public Exception
{
	public:
		using Exception::Exception;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
