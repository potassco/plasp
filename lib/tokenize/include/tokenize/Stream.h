#ifndef __TOKENIZE__STREAM_H
#define __TOKENIZE__STREAM_H

#include <experimental/filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <tokenize/Location.h>

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
		using Position = std::stringstream::pos_type;

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