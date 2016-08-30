#ifndef __PLASP__UTILS__PARSER_H
#define __PLASP__UTILS__PARSER_H

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <boost/filesystem.hpp>

#include <plasp/utils/ParserException.h>
#include <plasp/utils/ParserPolicy.h>
#include <plasp/utils/Stream.h>
#include <plasp/utils/StreamCoordinate.h>

namespace plasp
{
namespace utils
{

template<typename Type>
struct Tag
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parser
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy = CaseSensitiveParserPolicy>
class Parser: public Stream, public ParserPolicy
{
	template<class OtherParserPolicy>
	friend class Parser;

	public:
		explicit Parser();
		explicit Parser(std::string streamName, std::istream &istream);

		template<class OtherParser>
		Parser(OtherParser &&otherParser)
		{
			m_stream = std::move(otherParser.m_stream);
			m_delimiters = std::move(otherParser.m_delimiters);
		}

		void removeComments(const std::string &startSequence, const std::string &endSequence, bool removeEnd);

		char currentCharacter() const;

		template<typename Type>
		Type parse();

		template<typename Type>
		bool testAndReturn(const Type &expectedValue);

		template<typename Type>
		bool testAndSkip(const Type &expectedValue);

		template<typename Type>
		void expect(const Type &expectedValue);

		std::string parseIdentifier();
		bool testIdentifierAndReturn(const std::string &identifier);
		bool testIdentifierAndSkip(const std::string &identifier);

		// TODO: remove
		bool probeNumber();

		std::string parseLine();

		void skipWhiteSpace();
		void skipBlankSpace();
		void skipLine();

	private:
		std::string parseImpl(Tag<std::string>);
		char parseImpl(Tag<char>);
		uint64_t parseImpl(Tag<uint64_t>);
		int64_t parseImpl(Tag<int64_t>);
		uint32_t parseImpl(Tag<uint32_t>);
		int32_t parseImpl(Tag<int32_t>);
		bool parseImpl(Tag<bool>);

		bool testImpl(const std::string &expectedValue);
		bool testImpl(char expectedValue);
		bool testImpl(uint64_t expectedValue);
		bool testImpl(int64_t expectedValue);
		bool testImpl(uint32_t expectedValue);
		bool testImpl(int32_t expectedValue);
		bool testImpl(bool expectedValue);

		uint64_t parseIntegerBody();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
Parser<ParserPolicy>::Parser()
:	Stream()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
Parser<ParserPolicy>::Parser(std::string streamName, std::istream &istream)
:	Stream(streamName, istream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
void Parser<ParserPolicy>::skipWhiteSpace()
{
	check();

	while (!atEnd() && ParserPolicy::isWhiteSpaceCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
void Parser<ParserPolicy>::skipBlankSpace()
{
	check();

	while (!atEnd() && ParserPolicy::isBlankCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
void Parser<ParserPolicy>::skipLine()
{
	check();

	while (currentCharacter() != '\n')
		advance();

	advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
template<typename Type>
Type Parser<ParserPolicy>::parse()
{
	return parseImpl(Tag<Type>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
template<typename Type>
bool Parser<ParserPolicy>::testAndReturn(const Type &expectedValue)
{
	const auto previousPosition = position();

	const auto result = testImpl(expectedValue);

	seek(previousPosition);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
template<typename Type>
bool Parser<ParserPolicy>::testAndSkip(const Type &expectedValue)
{
	const auto previousPosition = position();

	const auto result = testImpl(expectedValue);

	if (result == false)
		seek(previousPosition);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
template<typename Type>
void Parser<ParserPolicy>::expect(const Type &expectedValue)
{
	if (testAndSkip(expectedValue))
		return;

	std::stringstream message;
	message << "unexpected value, expected “" << expectedValue << "”";

	throw ParserException(coordinate(), message.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
std::string Parser<ParserPolicy>::parseIdentifier()
{
	skipWhiteSpace();

	std::string value;

	while (true)
	{
		const auto character = currentCharacter();

		if (!ParserPolicy::isIdentifierCharacter(character))
			return value;

		value.push_back(character);
		advance();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testIdentifierAndSkip(const std::string &expectedValue)
{
	return testAndSkip(expectedValue) && !ParserPolicy::isIdentifierCharacter(currentCharacter());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::probeNumber()
{
	const auto previousPosition = position();

	skipWhiteSpace();

	while (!ParserPolicy::isWhiteSpaceCharacter(currentCharacter()))
		if (!std::isdigit(currentCharacter()))
		{
			seek(previousPosition);

			return false;
		}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
std::string Parser<ParserPolicy>::parseLine()
{
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

template<class ParserPolicy>
void Parser<ParserPolicy>::removeComments(const std::string &startSequence, const std::string &endSequence, bool removeEnd)
{
	const auto inPosition = m_stream.tellg();
	const auto outPosition = m_stream.tellp();

	m_stream.seekg(0);

	const auto removeRange =
		[&](const auto &start, const auto &end)
		{
			BOOST_ASSERT(start != -1);

			m_stream.clear();
			m_stream.seekp(start);
			m_stream.seekg(start);

			auto position = start;

			while (end == -1 || position < end)
			{
				m_stream.ignore(1);

				if (atEnd())
					return;

				m_stream.put(' ');
				position += static_cast<std::streamoff>(1);
			}
		};

	while (!atEnd())
	{
		Position startPosition = m_stream.tellg();

		while (!atEnd())
		{
			startPosition = m_stream.tellg();

			if (testAndSkip(startSequence))
				break;

			advance();
		}

		Position endPosition = m_stream.tellg();

		while (!atEnd())
		{
			endPosition = m_stream.tellg();

			if (testAndSkip(endSequence))
				break;

			advance();
		}

		if (removeEnd)
			endPosition = m_stream.tellg();

		removeRange(startPosition, endPosition);
	}

	m_stream.clear();

	m_stream.seekg(inPosition);
	m_stream.seekp(outPosition);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
char Parser<ParserPolicy>::currentCharacter() const
{
	return ParserPolicy::transformCharacter(Stream::currentCharacter());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
std::string Parser<ParserPolicy>::parseImpl(Tag<std::string>)
{
	skipWhiteSpace();

	const auto startPosition = position();

	while (!ParserPolicy::isWhiteSpaceCharacter(currentCharacter()))
		advance();

	const auto endPosition = position();
	const auto length = static_cast<size_t>(endPosition - startPosition);

	std::string value;
	value.reserve(length);

	seek(startPosition);

	for (size_t i = 0; i < length; i++)
	{
		value.push_back(currentCharacter());
		advance();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
char Parser<ParserPolicy>::parseImpl(Tag<char>)
{
	const auto value = currentCharacter();

	advance();

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
uint64_t Parser<ParserPolicy>::parseIntegerBody()
{
	check();

	if (!std::isdigit(currentCharacter()))
		throw ParserException(coordinate(), "could not parse integer value");

	uint64_t value = 0;

	while (!atEnd())
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

template<class ParserPolicy>
int64_t Parser<ParserPolicy>::parseImpl(Tag<int64_t>)
{
	skipWhiteSpace();

	bool positive = testAndSkip<char>('+') || !testAndSkip<char>('-');

	const auto value = parseIntegerBody();

	return (positive ? value : -value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
uint64_t Parser<ParserPolicy>::parseImpl(Tag<uint64_t>)
{
	skipWhiteSpace();

	if (currentCharacter() == '-')
		throw ParserException(coordinate(), "expected unsigned integer, got signed one");

	return parseIntegerBody();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
int32_t Parser<ParserPolicy>::parseImpl(Tag<int32_t>)
{
	return static_cast<int32_t>(parseImpl(Tag<int64_t>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
uint32_t Parser<ParserPolicy>::parseImpl(Tag<uint32_t>)
{
	return static_cast<uint32_t>(parseImpl(Tag<uint64_t>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::parseImpl(Tag<bool>)
{
	skipWhiteSpace();

	if (testAndSkip<char>('0'))
	    return false;

	if (testAndSkip<char>('1'))
		return true;

	throw ParserException(coordinate(), "could not parse Boolean value");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(const std::string &expectedValue)
{
	if (!ParserPolicy::isWhiteSpaceCharacter(expectedValue.front()))
		skipWhiteSpace();

	const auto match = std::find_if(expectedValue.cbegin(), expectedValue.cend(),
		[&](const auto &expectedCharacter)
		{
			const auto character = static_cast<char>(this->currentCharacter());

			if (character != expectedCharacter)
				return true;

			this->advance();

			return false;
		});

	return (match == expectedValue.cend());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(char expectedValue)
{
	const auto result = (currentCharacter() == expectedValue);

	advance();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(int64_t expectedValue)
{
	const auto value = parseImpl(Tag<int64_t>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(uint64_t expectedValue)
{
	const auto value = parseImpl(Tag<uint64_t>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(int32_t expectedValue)
{
	return testImpl(static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(uint32_t expectedValue)
{
	return testImpl(static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
bool Parser<ParserPolicy>::testImpl(bool expectedValue)
{
	const auto value = parseImpl(Tag<bool>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
