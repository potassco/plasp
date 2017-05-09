#ifndef __PARSE_BASE__STREAM_H
#define __PARSE_BASE__STREAM_H

#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <parsebase/Location.h>

namespace parsebase
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Stream
{
	public:
		using Position = std::stringstream::pos_type;

		struct Delimiter
		{
			Position position;
			std::string sectionName;
		};

	public:
		explicit Stream();
		explicit Stream(std::string streamName, std::istream &istream);

		Stream(const Stream &other) = delete;
		Stream &operator=(const Stream &other) = delete;

		Stream(Stream &&other)
		:	m_stream{std::move(other.m_stream)},
			m_delimiters{std::move(other.m_delimiters)}
		{
		}

		Stream &operator=(Stream &&other)
		{
			m_stream = std::move(other.m_stream);
			m_delimiters = std::move(other.m_delimiters);

			return *this;
		}

		~Stream() = default;

		void read(std::string streamName, std::istream &istream);
		void read(const std::experimental::filesystem::path &path);

		void reset();
		void seek(Position position);
		Position position() const;
		Location location() const;

		char currentCharacter() const;
		void advance();
		bool atEnd() const;

		void check() const;

	protected:
		mutable std::stringstream m_stream;

		std::vector<Delimiter> m_delimiters;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
