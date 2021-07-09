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
class Tokenizer : public Stream, public TokenizerPolicy
{
	template<class OtherTokenizerPolicy>
	friend class Tokenizer;

	public:
		explicit Tokenizer() noexcept;
		explicit Tokenizer(std::string streamName, std::istream &istream);

		template<class OtherTokenizer>
		Tokenizer(OtherTokenizer &&other) noexcept
		:	Stream(std::forward<OtherTokenizer>(other))
		{
		}

		void removeComments(const std::string &startSequence, const std::string &endSequence, bool removeEnd);

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
		bool testIdentifierAndSkip(const std::string &identifier);
		bool testIdentifierAndReturn(const std::string &identifier);

		// TODO: remove
		bool probeNumber();

		std::string getLine();

		void skipWhiteSpace();
		void skipBlankSpace();
		void skipLine();

	private:
		std::string getImpl(Tag<std::string>);
		char getImpl(Tag<char>);
		unsigned long long getImpl(Tag<unsigned long long>);
		unsigned long getImpl(Tag<unsigned long>);
		unsigned int getImpl(Tag<unsigned int>);
		signed long long getImpl(Tag<signed long long>);
		signed long getImpl(Tag<signed long>);
		signed int getImpl(Tag<signed int>);
		bool getImpl(Tag<bool>);

		bool testImpl(const std::string &expectedValue);
		bool testImpl(char expectedValue);
		bool testImpl(unsigned long long expectedValue);
		bool testImpl(signed long long expectedValue);
		bool testImpl(unsigned long expectedValue);
		bool testImpl(signed long expectedValue);
		bool testImpl(unsigned int expectedValue);
		bool testImpl(signed int expectedValue);
		bool testImpl(bool expectedValue);

		uint64_t getIntegerBody();
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
Tokenizer<TokenizerPolicy>::Tokenizer() noexcept
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
	while (!atEnd() && TokenizerPolicy::isWhiteSpaceCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::skipBlankSpace()
{
	while (!atEnd() && TokenizerPolicy::isBlankCharacter(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
void Tokenizer<TokenizerPolicy>::skipLine()
{
	while (!atEnd() && currentCharacter() != '\n')
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
		{
			if (value.empty())
				throw TokenizerException(location(), "could not parse identifier");

			return value;
		}

		value.push_back(character);
		advance();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testIdentifierAndReturn(const std::string &expectedValue)
{
	const auto previousPosition = position();

	const auto result = testIdentifierAndSkip(expectedValue);

	seek(previousPosition);

	return result;
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
	// TODO: move to appropriate place
	for (auto &character : m_content)
		character = TokenizerPolicy::transformCharacter(character);

	const auto removeRange =
		[&](const auto &start, const auto &end)
		{
			const auto previousPosition = position();

			assert(start < m_content.size());

			seek(start);

			while (position() < end)
			{
				if (atEnd())
					return;

				m_content[position()] = ' ';
				advanceUnchecked();
			}

			seek(previousPosition);
		};

	seek(0);

	// TODO: refactor
	while (!atEnd())
	{
		bool startSequenceFound = false;

		while (!atEnd())
		{
			if ((startSequenceFound = testAndSkip(startSequence)))
				break;

			advanceUnchecked();
		}

		if (!startSequenceFound && atEnd())
			break;

		const auto startPosition = position() - startSequence.size();

		bool endSequenceFound = false;

		while (!atEnd())
		{
			if ((endSequenceFound = testAndSkip(endSequence)))
				break;

			advanceUnchecked();
		}

		// If the end sequence is to be removed or could not be found, remove entire range
		const auto endPosition =
			(removeEnd || !endSequenceFound)
			? position()
			: position() - endSequence.size();

		removeRange(startPosition, endPosition);

		seek(endPosition + 1);
	}

	seek(0);
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

		advanceUnchecked();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
signed long long Tokenizer<TokenizerPolicy>::getImpl(Tag<signed long long>)
{
	skipWhiteSpace();

	bool positive = testAndSkip<char>('+') || !testAndSkip<char>('-');

	const auto value = getIntegerBody();

	return (positive ? value : -value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
signed long Tokenizer<TokenizerPolicy>::getImpl(Tag<signed long>)
{
	return static_cast<signed long>(getImpl(Tag<signed long long>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
signed int Tokenizer<TokenizerPolicy>::getImpl(Tag<signed int>)
{
	return static_cast<signed int>(getImpl(Tag<signed long long>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
unsigned long long Tokenizer<TokenizerPolicy>::getImpl(Tag<unsigned long long>)
{
	skipWhiteSpace();

	if (currentCharacter() == '-')
		throw TokenizerException(location(), "expected unsigned integer, got signed one");

	return getIntegerBody();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
unsigned long Tokenizer<TokenizerPolicy>::getImpl(Tag<unsigned long>)
{
	return static_cast<unsigned long>(getImpl(Tag<unsigned long long>()));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
unsigned int Tokenizer<TokenizerPolicy>::getImpl(Tag<unsigned int>)
{
	return static_cast<unsigned int>(getImpl(Tag<unsigned long long>()));
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

	for (size_t i = 0; i < expectedValue.size(); i++)
	{
		if (atEnd())
			return false;

		const auto character = currentCharacter();

		if (character != expectedValue[i])
			return false;

		advance();
	}

	return true;
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
bool Tokenizer<TokenizerPolicy>::testImpl(signed long long expectedValue)
{
	const auto value = getImpl(Tag<signed long long>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(unsigned long long expectedValue)
{
	const auto value = getImpl(Tag<unsigned long long>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(signed long expectedValue)
{
	const auto value = getImpl(Tag<signed long>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(unsigned long expectedValue)
{
	const auto value = getImpl(Tag<unsigned long>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(signed int expectedValue)
{
	const auto value = getImpl(Tag<signed int>());

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TokenizerPolicy>
bool Tokenizer<TokenizerPolicy>::testImpl(unsigned int expectedValue)
{
	const auto value = getImpl(Tag<unsigned int>());

	return (value == expectedValue);
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
