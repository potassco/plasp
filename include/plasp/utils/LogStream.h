#ifndef __PLASP__UTILS__LOG_STREAM_H
#define __PLASP__UTILS__LOG_STREAM_H

#include <iostream>
#include <unistd.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LogStream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class StandardStream
{
	Out,
	Err
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class LogStream
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
		LogStream(StandardStream standardStream)
		:	m_standardStream{standardStream},
			m_colorPolicy{ColorPolicy::Auto}
		{
		}

		LogStream(const LogStream &other)
		:	m_standardStream{other.m_standardStream},
			m_colorPolicy{ColorPolicy::Auto}
		{
		}

		LogStream &operator=(const LogStream &other)
		{
			m_standardStream = other.m_standardStream;
			m_colorPolicy = other.m_colorPolicy;

			return *this;
		}

		LogStream(LogStream &&other)
		:	m_standardStream{other.m_standardStream},
			m_colorPolicy{other.m_colorPolicy}
		{
			other.m_colorPolicy = ColorPolicy::Auto;
		}

		LogStream &operator=(LogStream &&other)
		{
			m_standardStream = other.m_standardStream;
			m_colorPolicy = other.m_colorPolicy;

			other.m_colorPolicy = ColorPolicy::Auto;

			return *this;
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

			// Autodetect by checking whether output goes to a terminal
			const auto fileDescriptor =
				(m_standardStream == utils::StandardStream::Out)
				? STDOUT_FILENO
				: STDERR_FILENO;

			return isatty(fileDescriptor);
		}

		std::ostream &ostream()
		{
			return (m_standardStream == utils::StandardStream::Out)
				? std::cout
				: std::cerr;
		}

		inline LogStream &operator<<(short value);
		inline LogStream &operator<<(unsigned short value);
		inline LogStream &operator<<(int value);
		inline LogStream &operator<<(unsigned int value);
		inline LogStream &operator<<(long value);
		inline LogStream &operator<<(unsigned long value);
		inline LogStream &operator<<(long long value);
		inline LogStream &operator<<(unsigned long long value);
		inline LogStream &operator<<(float value);
		inline LogStream &operator<<(double value);
		inline LogStream &operator<<(long double value);
		inline LogStream &operator<<(bool value);
		inline LogStream &operator<<(const void *value);
		inline LogStream &operator<<(const char *value);
		inline LogStream &operator<<(std::basic_streambuf<CharacterType, TraitsType> *sb);
		inline LogStream &operator<<(std::ios_base &(*func)(std::ios_base &));
		inline LogStream &operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &));
		inline LogStream &operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &));

	private:
		StandardStream m_standardStream;
		ColorPolicy m_colorPolicy;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(short value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(unsigned short value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(int value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(unsigned int value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(unsigned long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(long long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(unsigned long long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(float value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(double value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(long double value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(bool value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(const void *value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(const char *value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(std::basic_streambuf<CharacterType, TraitsType>* sb)
{
	ostream() << sb;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(std::ios_base &(*func)(std::ios_base &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

LogStream &LogStream::operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharacterType, class Traits, class Allocator>
inline LogStream &operator<<(LogStream &stream, const std::basic_string<CharacterType, Traits, Allocator> &string)
{
	stream.ostream() << string;
	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
