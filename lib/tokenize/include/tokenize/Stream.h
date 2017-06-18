#ifndef __TOKENIZE__STREAM_H
#define __TOKENIZE__STREAM_H

#include <cassert>
#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <tokenize/Location.h>
#include <tokenize/TokenizerException.h>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
	public:
		using Position = size_t;
		static const Position InvalidPosition;

		struct Delimiter
		{
			Position position;
			std::string sectionName;
		};

	public:
		Stream();
		explicit Stream(std::string streamName, std::istream &istream);
		~Stream() = default;

		Stream(const Stream &other) = delete;
		Stream &operator=(const Stream &other) = delete;
		Stream(Stream &&other) = default;
		Stream &operator=(Stream &&other) = default;

		void read(std::string streamName, std::istream &istream);
		void read(const std::experimental::filesystem::path &path);

		void reset();
		void seek(Position position);
		Position position() const;
		Location location() const;

		char currentCharacter() const
		{
			assert(m_position < m_stream.size());

			// TODO: check if this should be secured by check()
			return m_stream[m_position];
		}

		void advance()
		{
			check();
			m_position++;
		}

		bool atEnd() const
		{
			return m_position >= m_stream.size();
		}

		void check() const
		{
			if (atEnd())
				throw TokenizerException(location(), "reading past end of file");
		}

	protected:
		std::string m_stream;
		mutable Position m_position;

		std::vector<Delimiter> m_delimiters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
