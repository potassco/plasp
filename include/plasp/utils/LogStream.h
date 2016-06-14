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

template<StandardStream StandardStream>
class LogStream
{
	private:
		using CharacterType = std::ostream::char_type;
		using TraitsType = std::ostream::traits_type;

	public:
		bool supportsColor() const
		{
			const auto fileDescriptor =
				(StandardStream == StandardStream::Out)
				? STDOUT_FILENO
				: STDERR_FILENO;

			return isatty(fileDescriptor);
		}

		std::ostream &ostream()
		{
			return (StandardStream == StandardStream::Out)
				? std::cout
				: std::cerr;
		}

		LogStream &operator<<(short value);
		LogStream &operator<<(unsigned short value);
		LogStream &operator<<(int value);
		LogStream &operator<<(unsigned int value);
		LogStream &operator<<(long value);
		LogStream &operator<<(unsigned long value);
		LogStream &operator<<(long long value);
		LogStream &operator<<(unsigned long long value);
		LogStream &operator<<(float value);
		LogStream &operator<<(double value);
		LogStream &operator<<(long double value);
		LogStream &operator<<(bool value);
		LogStream &operator<<(const void *value);
		LogStream &operator<<(const char *value);
		LogStream &operator<<(std::basic_streambuf<CharacterType, TraitsType> *sb);
		LogStream &operator<<(std::ios_base &(*func)(std::ios_base &));
		LogStream &operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &));
		LogStream &operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &));
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(short value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(unsigned short value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(int value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(unsigned int value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(unsigned long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(long long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(unsigned long long value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(float value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(double value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(long double value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(bool value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(const void *value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(const char *value)
{
	ostream() << value;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(std::basic_streambuf<CharacterType, TraitsType>* sb)
{
	ostream() << sb;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(std::ios_base &(*func)(std::ios_base &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(std::basic_ios<CharacterType, TraitsType> &(*func)(std::basic_ios<CharacterType, TraitsType> &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream>
LogStream<StandardStream> &LogStream<StandardStream>::operator<<(std::basic_ostream<CharacterType, TraitsType> &(*func)(std::basic_ostream<CharacterType, TraitsType> &))
{
	ostream() << func;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<StandardStream StandardStream, class CharacterType, class Traits, class Allocator>
LogStream<StandardStream> &operator<<(LogStream<StandardStream> &stream, const std::basic_string<CharacterType, Traits, Allocator> &string)
{
	stream.ostream() << string;
	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
