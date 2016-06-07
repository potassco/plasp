#include <plasp/utils/Parser.h>

#include <algorithm>
#include <fstream>

#include <boost/assert.hpp>

#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parser
//
////////////////////////////////////////////////////////////////////////////////////////////////////

const std::istreambuf_iterator<char> Parser::EndOfFile = std::istreambuf_iterator<char>();

////////////////////////////////////////////////////////////////////////////////////////////////////

Parser::Parser()
:	m_isCaseSensitive{true}
{
	std::setlocale(LC_NUMERIC, "C");

	// Don’t skip whitespace
	m_stream.exceptions(std::istream::badbit);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Parser::Parser(std::string streamName, std::istream &istream)
:	Parser()
{
	readStream(streamName, istream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::readStream(std::string streamName, std::istream &istream)
{
	// Store position of new section
	const auto position = m_stream.tellp();

	m_streamDelimiters.push_back({position, streamName});

	m_stream << istream.rdbuf();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::readFile(const boost::filesystem::path &path)
{
	if (!boost::filesystem::is_regular_file(path))
		throw std::runtime_error("File does not exist: \"" + path.string() + "\"");

	std::ifstream fileStream(path.string(), std::ios::in);

	readStream(path.string(), fileStream);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::reset()
{
	m_stream.clear();
	seek(std::ios::beg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::seek(Position position)
{
	m_stream.clear();
	m_stream.seekg(position);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Parser::Position Parser::position() const
{
	return m_stream.tellg();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

Parser::Coordinate Parser::coordinate() const
{
	const auto currentPosition = position();

	// Find current section
	auto currentFile = std::find_if(m_streamDelimiters.crbegin(), m_streamDelimiters.crend(),
		[&](const auto &fileDelimiter)
		{
			return currentPosition >= fileDelimiter.position;
		});

	// If the parser is at the end of the stream, still count from the beginning of the last section
	if (currentFile == m_streamDelimiters.crend())
		currentFile = m_streamDelimiters.crbegin();

	// Go back to beginning of section
	m_stream.clear();
	m_stream.seekg(currentFile->position);

	size_t row = 1;
	size_t column = 1;

	// Compute the coordinate character by character
	while (true)
	{
		if (currentPosition == -1 && atEndOfStream())
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

void Parser::setCaseSensitive(bool isCaseSensitive)
{
	m_isCaseSensitive = isCaseSensitive;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

char Parser::currentCharacter() const
{
	if (m_isCaseSensitive)
		return m_stream.peek();

	return std::tolower(m_stream.peek());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Parser::atEndOfStream() const
{
	return position() == -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::checkStream() const
{
	if (atEndOfStream())
		throw ParserException(*this, "Reading past end of file");

	if (m_stream.fail())
		throw ParserException(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::advance()
{
	checkStream();
	m_stream.ignore(1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool Parser::advanceIf(char expectedCharacter)
{
	checkStream();

	if (currentCharacter() != expectedCharacter)
		return false;

	advance();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::skipWhiteSpace()
{
	return skipWhiteSpace(
		[](const auto character)
		{
			return std::isspace(character);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void Parser::skipLine()
{
	checkStream();

	while (currentCharacter() != '\n')
		advance();

	advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Parser::getLine()
{
	checkStream();

	std::string value;

	while (true)
	{
		const auto character = currentCharacter();

		advance();

		if (character == '\n')
			break;
		else if (character == '\r')
			continue;

		value.push_back(character);
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
std::string Parser::parse<std::string>()
{
	skipWhiteSpace();

	std::string value;

	while (true)
	{
		const auto character = currentCharacter();

		if (std::isspace(character))
			break;

		value.push_back(character);
		advance();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<std::string>(const std::string &expectedValue)
{
	BOOST_ASSERT(!std::isspace(expectedValue[0]));

	skipWhiteSpace();

	std::for_each(expectedValue.cbegin(), expectedValue.cend(),
		[&](const auto &expectedCharacter)
		{
			const auto character = static_cast<char>(this->currentCharacter());

			if (character != expectedCharacter)
				throw ParserException(*this, "Unexpected string, expected \"" + expectedValue + "\" (expected character '" + expectedCharacter + "', got '" + character + "')");

			this->advance();
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

uint64_t Parser::parseIntegerBody()
{
	checkStream();

	if (!std::isdigit(currentCharacter()))
		throw ParserException(*this, "Could not parse integer value");

	uint64_t value = 0;

	while (!atEndOfStream())
	{
		const auto character = currentCharacter();

		if (!std::isdigit(character))
			break;

		value *= 10;
		value += character - '0';

		advance();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
int64_t Parser::parse<int64_t>()
{
	skipWhiteSpace();

	bool positive = advanceIf('+') || !advanceIf('-');

	const auto value = parseIntegerBody();

	return (positive ? value : -value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
uint64_t Parser::parse<uint64_t>()
{
	skipWhiteSpace();

	if (currentCharacter() == '-')
		throw ParserException(*this, "Expected unsigned integer, got signed one");

	return parseIntegerBody();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<int64_t>(const int64_t &expectedValue)
{
	const auto value = parse<int64_t>();

	if (value != expectedValue)
		throw ParserException(*this, "Unexpected value " + std::to_string(value) + ", expected " + std::to_string(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<uint64_t>(const uint64_t &expectedValue)
{
	const auto value = parse<uint64_t>();

	if (value != expectedValue)
		throw ParserException(*this, "Unexpected value " + std::to_string(value) + ", expected " + std::to_string(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
int32_t Parser::parse<int32_t>()
{
	return static_cast<int32_t>(parse<int64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
uint32_t Parser::parse<uint32_t>()
{
	return static_cast<uint32_t>(parse<uint64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<int32_t>(const int32_t &expectedValue)
{
	expect<int64_t>(static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<uint32_t>(const uint32_t &expectedValue)
{
	expect<uint64_t>(static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
int16_t Parser::parse<int16_t>()
{
	return static_cast<int16_t>(parse<int64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
uint16_t Parser::parse<uint16_t>()
{
	return static_cast<uint16_t>(parse<uint64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<int16_t>(const int16_t &expectedValue)
{
	expect<int64_t>(static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<uint16_t>(const uint16_t &expectedValue)
{
	expect<uint64_t>(static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
int8_t Parser::parse<int8_t>()
{
	return static_cast<int8_t>(parse<int64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
uint8_t Parser::parse<uint8_t>()
{
	return static_cast<uint8_t>(parse<uint64_t>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<int8_t>(const int8_t &expectedValue)
{
	expect<int64_t>(static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<uint8_t>(const uint8_t &expectedValue)
{
	expect<uint64_t>(static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
bool Parser::parse<bool>()
{
	skipWhiteSpace();

	if (advanceIf('0'))
	    return false;

	if (advanceIf('1'))
		return true;

	throw ParserException(*this, "Could not parse Boolean value");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<>
void Parser::expect<bool>(const bool &expectedValue)
{
	const auto value = parse<bool>();

	if (value != expectedValue)
		throw ParserException(*this, "Unexpected value " + std::to_string(value) + ", expected " + std::to_string(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
