#include <tokenize/Stream.h>

#include <algorithm>
#include <fstream>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const Stream::Position Stream::InvalidPosition{std::numeric_limits<Position>::max()};

////////////////////////////////////////////////////////////////////////////////////////////////////

Stream::Stream()
:	m_position{0}
{
	std::setlocale(LC_NUMERIC, "C");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Stream::Stream(std::string streamName, std::istream &istream)
{
	read(streamName, istream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::read(std::string streamName, std::istream &istream)
{
	// Store position of new section
	const auto position = m_stream.size();

	m_delimiters.push_back({position, streamName});

	istream.seekg(0, std::ios::end);
	const auto streamSize = istream.tellg();
	istream.seekg(0, std::ios::beg);

	const auto startPosition = m_stream.size();

	m_stream.resize(m_stream.size() + streamSize);
	std::copy(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>(), m_stream.begin() + startPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::read(const std::experimental::filesystem::path &path)
{
	if (!std::experimental::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: “" + path.string() + "”");

	std::ifstream fileStream(path.string(), std::ios::in);

	read(path.string(), fileStream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::reset()
{
	m_position = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::seek(Position position)
{
	m_position = position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

typename Stream::Position Stream::position() const
{
	return m_position;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Location Stream::location() const
{
	const auto currentPosition = position();

	// Find current section
	auto currentFile = std::find_if(m_delimiters.crbegin(), m_delimiters.crend(),
		[&](const auto &fileDelimiter)
		{
			return currentPosition >= fileDelimiter.position;
		});

	// If the tokenizer is at the end of the stream, still count from the beginning of the last section
	if (currentFile == m_delimiters.crend())
		currentFile = m_delimiters.crbegin();

	// Go back to beginning of section
	m_position = currentFile->position;

	size_t row = 1;
	size_t column = 1;

	// Compute the location character by character
	while (true)
	{
		if (currentPosition >= m_stream.size() && atEnd())
			break;
		else if (currentPosition < m_stream.size() && position() >= currentPosition)
			break;

		const auto character = currentCharacter();

		if (character == '\n')
		{
			row++;
			column = 1;
		}
		else if (std::isblank(character) || std::isprint(character))
			column++;

		m_position++;
	}

	return {currentFile->sectionName.c_str(), currentFile->sectionName.c_str(), row, row, column, column};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
