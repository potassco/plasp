#ifndef __PLASP__TESTS__NULL_OUTPUT_STREAM_H
#define __PLASP__TESTS__NULL_OUTPUT_STREAM_H

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NullOutputStream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class NullStreamBuffer : public std::streambuf
{
	private:
		char dummyBuffer[64];

	protected:
		virtual int overflow(int c)
		{
			setp(dummyBuffer, dummyBuffer + sizeof(dummyBuffer));
			return (c == traits_type::eof()) ? '\0' : c;
		}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class NullOutputStream : public std::ostream
{
	public:
		NullOutputStream()
		:	std::ostream(&m_nullStreamBuffer)
		{
		}

		NullStreamBuffer *rdbuf() const
		{
			return &m_nullStreamBuffer;
		}

	private:
		mutable NullStreamBuffer m_nullStreamBuffer;
};

#endif
