#ifndef __TOKENIZE__TOKENIZER_H
#define __TOKENIZE__TOKENIZER_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <tokenize/TokenizerException.h>
#include <tokenize/TokenizerPolicy.h>
#include <tokenize/Stream.h>

namespace tokenize
{

template<typename Type>
struct Tag
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tokenizer
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy = CaseSensitiveTokenizerPolicy>
class Tokenizer: public Stream, public TokenizerPolicy
{
	template<class OtherTokenizerPolicy>
	friend class Tokenizer;

	public:
		explicit Tokenizer();
		explicit Tokenizer(std::string streamName, std::istream &istream);

		template<class OtherTokenizer>
		Tokenizer(OtherTokenizer &&otherTokenizer)
		{
			m_stream = std::move(otherTokenizer.m_stream);
			m_delimiters = std::move(otherTokenizer.m_delimiters);
		}

		void removeComments(const std::string &startSequence, const std::string &endSequence, bool removeEnd);

		char currentCharacter() const;

		template<typename Type>
		Type get();

		template<typename Type>
		bool testAndReturn(const Type &expectedValue);

		template<typename Type>
		bool testAndSkip(const Type &expectedValue);

		template<typename Type>
		void expect(const Type &expectedValue);

		// TODO: refactor
		std::string getIdentifier();
		bool testIdentifierAndReturn(const std::string &identifier);
		bool testIdentifierAndSkip(const std::string &identifier);

		// TODO: remove
		bool probeNumber();

		std::string getLine();

		void skipWhiteSpace();
		void skipBlankSpace();
		void skipLine();

	private:
		std::string getImpl(Tag<std::string>);
		char getImpl(Tag<char>);
		uint64_t getImpl(Tag<uint64_t>);
		int64_t getImpl(Tag<int64_t>);
		uint32_t getImpl(Tag<uint32_t>);
		int32_t getImpl(Tag<int32_t>);
		bool getImpl(Tag<bool>);

		bool testImpl(const std::string &expectedValue);
		bool testImpl(char expectedValue);
		bool testImpl(uint64_t expectedValue);
		bool testImpl(int64_t expectedValue);
		bool testImpl(uint32_t expectedValue);
		bool testImpl(int32_t expectedValue);
		bool testImpl(bool expectedValue);

		uint64_t getIntegerBody();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
Tokenizer<TokenizerPolicy>::Tokenizer()
:	Stream()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
Tokenizer<TokenizerPolicy>::Tokenizer(std::string streamName, std::istream &istream)
:	Stream(streamName, istream)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::skipWhiteSpace()
{
	check();

	while (!atEnd() && TokenizerPolicy::isWhiteSpaceCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::skipBlankSpace()
{
	check();

	while (!atEnd() && TokenizerPolicy::isBlankCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::skipLine()
{
	check();

	while (currentCharacter() != '\n')
		advance();

	advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
template<typename Type>
Type Tokenizer<TokenizerPolicy>::get()
{
	return getImpl(Tag<Type>());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
template<typename Type>
bool Tokenizer<TokenizerPolicy>::testAndReturn(const Type &expectedValue)
{
	const auto previousPosition = position();

	const auto result = testImpl(expectedValue);

	seek(previousPosition);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
template<typename Type>
bool Tokenizer<TokenizerPolicy>::testAndSkip(const Type &expectedValue)
{
	const auto previousPosition = position();

	const auto result = testImpl(expectedValue);

	if (result == false)
		seek(previousPosition);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
template<typename Type>
void Tokenizer<TokenizerPolicy>::expect(const Type &expectedValue)
{
	if (testAndSkip(expectedValue))
		return;

	std::stringstream message;
	message << "unexpected value, expected “" << expectedValue << "”";

	throw TokenizerException(location(), message.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
std::string Tokenizer<TokenizerPolicy>::getIdentifier()
{
	skipWhiteSpace();

	std::string value;

	while (true)
	{
		const auto character = currentCharacter();

		if (!TokenizerPolicy::isIdentifierCharacter(character))
			return value;

		value.push_back(character);
		advance();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testIdentifierAndSkip(const std::string &expectedValue)
{
	return testAndSkip(expectedValue) && !TokenizerPolicy::isIdentifierCharacter(currentCharacter());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::probeNumber()
{
	const auto previousPosition = position();

	skipWhiteSpace();

	while (!TokenizerPolicy::isWhiteSpaceCharacter(currentCharacter()))
		if (!std::isdigit(currentCharacter()))
		{
			seek(previousPosition);

			return false;
		}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
std::string Tokenizer<TokenizerPolicy>::getLine()
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

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::removeComments(const std::string &startSequence, const std::string &endSequence, bool removeEnd)
{
	const auto inPosition = m_stream.tellg();
	const auto outPosition = m_stream.tellp();

	m_stream.seekg(0);

	const auto removeRange =
		[&](const auto &start, const auto &end)
		{
			assert(start != -1);

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

template<class TokenizerPolicy>
char Tokenizer<TokenizerPolicy>::currentCharacter() const
{
	return TokenizerPolicy::transformCharacter(Stream::currentCharacter());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
std::string Tokenizer<TokenizerPolicy>::getImpl(Tag<std::string>)
{
	skipWhiteSpace();

	const auto startPosition = position();

	while (!TokenizerPolicy::isWhiteSpaceCharacter(currentCharacter()))
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

template<class TokenizerPolicy>
char Tokenizer<TokenizerPolicy>::getImpl(Tag<char>)
{
	const auto value = currentCharacter();

	advance();

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
uint64_t Tokenizer<TokenizerPolicy>::getIntegerBody()
{
	check();

	if (!std::isdigit(currentCharacter()))
		throw TokenizerException(location(), "could not read integer value");

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

template<class TokenizerPolicy>
int64_t Tokenizer<TokenizerPolicy>::getImpl(Tag<int64_t>)
{
	skipWhiteSpace();

	bool positive = testAndSkip<char>('+') || !testAndSkip<char>('-');

	const auto value = getIntegerBody();

	return (positive ? value : -value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
uint64_t Tokenizer<TokenizerPolicy>::getImpl(Tag<uint64_t>)
{
	skipWhiteSpace();

	if (currentCharacter() == '-')
		throw TokenizerException(location(), "expected unsigned integer, got signed one");

	return getIntegerBody();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
int32_t Tokenizer<TokenizerPolicy>::getImpl(Tag<int32_t>)
{
	return static_cast<int32_t>(getImpl(Tag<int64_t>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
uint32_t Tokenizer<TokenizerPolicy>::getImpl(Tag<uint32_t>)
{
	return static_cast<uint32_t>(getImpl(Tag<uint64_t>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::getImpl(Tag<bool>)
{
	skipWhiteSpace();

	if (testAndSkip<char>('0'))
	    return false;

	if (testAndSkip<char>('1'))
		return true;

	throw TokenizerException(location(), "could not read Boolean value");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(const std::string &expectedValue)
{
	if (!TokenizerPolicy::isWhiteSpaceCharacter(expectedValue.front()))
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

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(char expectedValue)
{
	const auto result = (currentCharacter() == expectedValue);

	advance();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(int64_t expectedValue)
{
	const auto value = getImpl(Tag<int64_t>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(uint64_t expectedValue)
{
	const auto value = getImpl(Tag<uint64_t>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(int32_t expectedValue)
{
	return testImpl(static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(uint32_t expectedValue)
{
	return testImpl(static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(bool expectedValue)
{
	const auto value = getImpl(Tag<bool>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
