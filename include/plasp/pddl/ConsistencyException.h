#ifndef __PLASP__PDDL__CONSISTENCY_EXCEPTION_H
#define __PLASP__PDDL__CONSISTENCY_EXCEPTION_H

#include <exception>
#include <string>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ConsistencyException
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ConsistencyException: public std::exception
{
	public:
		explicit ConsistencyException()
		:	ConsistencyException("unspecified consistency error")
		{
		}

		explicit ConsistencyException(const char *message)
		:	ConsistencyException(static_cast<std::string>(message))
		{
		}

		explicit ConsistencyException(const std::string &message)
		:	m_message{message}
		{
		}

		~ConsistencyException() throw()
		{
		}

		const char *what() const throw()
		{
			if (m_message.empty())
				return "unspecified consistency error";

			return m_message.c_str();
		}

	private:
		std::string m_message;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
