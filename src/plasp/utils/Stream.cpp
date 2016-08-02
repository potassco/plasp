#include <plasp/utils/Stream.h>

#include <fstream>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Stream
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Stream::Stream()
{
	std::setlocale(LC_NUMERIC, "C");

	// Don’t skip whitespace
	m_stream.exceptions(std::istream::badbit);
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
	const auto position = m_stream.tellp();

	m_delimiters.push_back({position, streamName});

	m_stream << istream.rdbuf();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::read(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: “" + path.string() + "”");

	std::ifstream fileStream(path.string(), std::ios::in);

	read(path.string(), fileStream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::reset()
{
	m_stream.clear();
	seek(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::seek(Position position)
{
	m_stream.clear();
	m_stream.seekg(position);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

typename Stream::Position Stream::position() const
{
	return m_stream.tellg();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

StreamCoordinate Stream::coordinate() const
{
	const auto currentPosition = position();

	// Find current section
	auto currentFile = std::find_if(m_delimiters.crbegin(), m_delimiters.crend(),
		[&](const auto &fileDelimiter)
		{
			return currentPosition >= fileDelimiter.position;
		});

	// If the parser is at the end of the stream, still count from the beginning of the last section
	if (currentFile == m_delimiters.crend())
		currentFile = m_delimiters.crbegin();

	// Go back to beginning of section
	m_stream.clear();
	m_stream.seekg(currentFile->position);

	size_t row = 1;
	size_t column = 1;

	// Compute the coordinate character by character
	while (true)
	{
		if (currentPosition == -1 && atEnd())
			break;
		else if (currentPosition >= 0 && position() >= currentPosition)
			break;

		const auto character = currentCharacter();

		if (character == '\n')
		{
			row++;
			column = 1;
		}
		else if (std::isblank(character) || std::isprint(character))
			column++;

		m_stream.ignore(1);
	}

	return {currentFile->sectionName, row, column};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Stream::currentCharacter() const
{
	return m_stream.peek();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Stream::atEnd() const
{
	return position() == -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::check() const
{
	if (atEnd())
		throw ParserException(coordinate(), "reading past end of file");

	if (m_stream.fail())
		throw ParserException(coordinate());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Stream::advance()
{
	check();
	m_stream.ignore(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
