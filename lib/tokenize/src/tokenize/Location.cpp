#include <tokenize/Location.h>

#include <algorithm>

#include <tokenize/Stream.h>

namespace tokenize
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Location
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Location::Location(Stream &stream)
:	m_stream{stream},
	m_position{stream.position()}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Location::Location(Stream &stream, StreamPosition position)
:	m_stream{stream},
	m_position{position}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *Location::sectionStart() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_sectionStart;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *Location::sectionEnd() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_sectionEnd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

StreamPosition Location::rowStart() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_rowStart;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

StreamPosition Location::rowEnd() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_rowEnd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

StreamPosition Location::columnStart() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_columnStart;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

StreamPosition Location::columnEnd() const
{
	if (!m_isInitialized)
		initializeLazily();

	return m_columnEnd;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Location::initializeLazily() const
{
	const auto previousPosition = m_stream.position();

	const auto &delimiters = m_stream.delimiters();

	// Find current section
	auto currentFile = std::find_if(delimiters.crbegin(), delimiters.crend(),
		[&](const auto &fileDelimiter)
		{
			return m_position >= fileDelimiter.position;
		});

	// If the tokenizer is at the end of the stream, still count from the beginning of the last section
	if (currentFile == delimiters.crend())
		currentFile = delimiters.crbegin();

	// Go back to beginning of section
	m_stream.seek(currentFile->position);

	StreamPosition row{1};
	StreamPosition column{1};

	// Compute the location character by character
	while (true)
	{
		if (m_stream.atEnd())
			break;
		else if (m_stream.position() >= m_position)
			break;

		const auto character = m_stream.currentCharacter();

		if (character == '\n')
		{
			row++;
			column = 1;
		}
		else if (std::isblank(character) || std::isprint(character))
			column++;

		m_stream.advance();
	}

	m_sectionStart = currentFile->sectionName.c_str();
	m_sectionEnd = currentFile->sectionName.c_str();
	m_rowStart = row;
	m_rowEnd = row;
	m_columnStart = column;
	m_columnEnd = column;

	m_isInitialized = true;

	m_stream.seek(previousPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
