#ifndef __TOKENIZE__STREAM_H
#define __TOKENIZE__STREAM_H

#include <cassert>
#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <tokenize/Location.h>
#include <tokenize/StreamPosition.h>
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
		struct Delimiter
		{
			StreamPosition position;
			std::string sectionName;
		};

	public:
		Stream();
		explicit Stream(std::string streamName, std::istream &istream);
		~Stream() = default;

		Stream(const Stream &other) = delete;
		Stream &operator=(const Stream &other) = delete;
		Stream(Stream &&other) = delete;
		Stream &operator=(Stream &&other) = delete;

		void read(std::string streamName, std::istream &istream);
		void read(const std::experimental::filesystem::path &path);

		void reset();
		void seek(StreamPosition position);
		StreamPosition position() const;

		const std::vector<Delimiter> &delimiters() const
		{
			return m_delimiters;
		}

		char currentCharacter()
		{
			check();
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

		void check()
		{
			if (atEnd())
				throw TokenizerException(*this, "reading past end of file");
		}

	protected:
		std::string m_stream;
		mutable StreamPosition m_position{0};

		std::vector<Delimiter> m_delimiters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
