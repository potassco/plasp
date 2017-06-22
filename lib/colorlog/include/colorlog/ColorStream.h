#ifndef __COLOR_LOG__COLOR_STREAM_H
#define __COLOR_LOG__COLOR_STREAM_H

#include <iostream>
#include <unistd.h>

namespace colorlog
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ColorStream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ColorStream
{
	public:
		enum class ColorPolicy
		{
			Never,
			Auto,
			Always
		};

	private:
		using CharacterType = std::ostream::char_type;
		using TraitsType = std::ostream::traits_type;

	public:
		ColorStream(std::ostream &stream)
		:	m_stream{stream},
			m_colorPolicy{ColorPolicy::Auto}
		{
		}

		void setColorPolicy(ColorPolicy colorPolicy)
		{
			m_colorPolicy = colorPolicy;
		}

		bool supportsColor() const
		{
			if (m_colorPolicy == ColorPolicy::Never)
				return false;

			if (m_colorPolicy == ColorPolicy::Always)
				return true;

			if (&m_stream == &std::cout)
				return isatty(fileno(stdout));

			if (&m_stream == &std::cerr)
				return isatty(fileno(stderr));

			return false;
		}

		std::ostream &stream()
		{
			return m_stream;
		}

		inline ColorStream &operator<<(short value);
		inline ColorStream &operator<<(unsigned short value);
		inline ColorStream &operator<<(int value);
		inline ColorStream &operator<<(unsigned int value);
		inline ColorStream &operator<<(long value);
		inline ColorStream &operator<<(unsigned long value);
		inline ColorStream &operator<<(long long value);
		inline ColorStream &operator<<(unsigned long long value);
		inline ColorStream &operator<<(float value);
		inline ColorStream &operator<<(double value);
		inline ColorStream &operator<<(long double value);
		inline ColorStream &operator<<(bool value);
		inline ColorStream &operator<<(const void *value);
		inline ColorStream &operator<<(const char *value);
		inline ColorStream &operator<<(const signed char *value);
		inline ColorStream &operator<<(const unsigned char *value);
		inline ColorStream &operator<<(std::basic_streambuf<CharacterType, TraitsType> *sb);
		inline ColorStream &operator<<(std::ios_base &(*func)(std::ios_base &));
		inline ColorStream &operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &));
		inline ColorStream &operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &));

		inline ColorStream &operator<<(char value);
		inline ColorStream &operator<<(signed char value);
		inline ColorStream &operator<<(unsigned char value);

	private:
		std::ostream &m_stream;
		ColorPolicy m_colorPolicy;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(short value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(unsigned short value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(int value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(unsigned int value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(long value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(unsigned long value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(long long value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(unsigned long long value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(float value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(double value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(long double value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(bool value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(const void *value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(const char *value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(const signed char *value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(const unsigned char *value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(std::basic_streambuf<CharacterType, TraitsType>* sb)
{
	m_stream << sb;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(std::ios_base &(*func)(std::ios_base &))
{
	m_stream << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &))
{
	m_stream << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &))
{
	m_stream << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharacterType, class Traits, class Allocator>
inline ColorStream &operator<<(ColorStream &colorStream, const std::basic_string<CharacterType, Traits, Allocator> &string)
{
	colorStream.stream() << string;
	return colorStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(char value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(signed char value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ColorStream &ColorStream::operator<<(unsigned char value)
{
	m_stream << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
