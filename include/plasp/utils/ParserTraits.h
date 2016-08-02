#ifndef __PLASP__UTILS__PARSER_TRAITS_H
#define __PLASP__UTILS__PARSER_TRAITS_H

#include <algorithm>
#include <sstream>
#include <string>

namespace plasp
{
namespace utils
{
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ParserTraits
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy, typename Type>
class ParserTraits
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, std::string>
{
	public:
		static std::string parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const std::string &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const std::string &value);
		static void expect(Parser<ParserPolicy> &parser, const std::string &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const std::string &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, char>
{
	public:
		static char parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const char &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const char &value);
		static void expect(Parser<ParserPolicy> &parser, const char &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const char &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, uint64_t>
{
	public:
		static uint64_t parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const uint64_t &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const uint64_t &value);
		static void expect(Parser<ParserPolicy> &parser, const uint64_t &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const uint64_t &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, int64_t>
{
	public:
		static int64_t parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const int64_t &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const int64_t &value);
		static void expect(Parser<ParserPolicy> &parser, const int64_t &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const int64_t &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, uint32_t>
{
	public:
		static uint32_t parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const uint32_t &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const uint32_t &value);
		static void expect(Parser<ParserPolicy> &parser, const uint32_t &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const uint32_t &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, int32_t>
{
	public:
		static int32_t parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const int32_t &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const int32_t &value);
		static void expect(Parser<ParserPolicy> &parser, const int32_t &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const int32_t &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
class ParserTraits<ParserPolicy, bool>
{
	public:
		static bool parse(Parser<ParserPolicy> &parser);
		static bool testAndReturn(Parser<ParserPolicy> &parser, const bool &value);
		static bool testAndSkip(Parser<ParserPolicy> &parser, const bool &value);
		static void expect(Parser<ParserPolicy> &parser, const bool &expectedValue);
		static bool test(Parser<ParserPolicy> &parser, const bool &value);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser, typename Type>
bool ParserTraits<Parser, Type>::testAndReturn(Parser &parser, const Type &value)
{
	const auto position = parser.position();

	const auto result = test(parser, value);

	parser.seek(position);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser, typename Type>
bool ParserTraits<Parser, Type>::testAndSkip(Parser &parser, const Type &value)
{
	const auto position = parser.position();

	const auto result = test(parser, value);

	if (result == false)
		parser.seek(position);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class ParserPolicy>
void Parser<ParserPolicy>::expect(const Type &expectedValue)
{
	if (testAndSkip(parser, expectedValue))
		return;

	std::stringstream message;
	message << "unexpected value, expected “" << expectedValue << "”";

	throw ParserException(parser.coordinate(), message.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
std::string ParserTraits<Parser, std::string>::parse(Parser &parser)
{
	parser.skipWhiteSpace();

	const auto startPosition = parser.position();

	while (!parser.isWhiteSpace(parser.currentCharacter()))
		parser.advance();

	const auto endPosition = parser.position();
	const auto length = static_cast<size_t>(endPosition - startPosition);

	std::string value;
	value.reserve(length);

	parser.seek(startPosition);

	for (size_t i = 0; i < length; i++)
	{
		value.push_back(parser.currentCharacter());
		parser.advance();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
char ParserTraits<Parser, char>::parse(Parser &parser)
{
	const auto value = parser.currentCharacter();

	parser.advance();

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
uint64_t parseIntegerBody(Parser &parser)
{
	parser.checkStream();

	if (!std::isdigit(parser.currentCharacter()))
		throw ParserException(parser.coordinate(), "could not parse integer value");

	uint64_t value = 0;

	while (!parser.atEndOfStream())
	{
		const auto character = parser.currentCharacter();

		if (!std::isdigit(character))
			break;

		value *= 10;
		value += character - '0';

		parser.advance();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
int64_t ParserTraits<Parser, int64_t>::parse(Parser &parser)
{
	parser.skipWhiteSpace();

	bool positive = parser.template testAndSkip<char>('+') || !parser.template testAndSkip<char>('-');

	const auto value = parseIntegerBody(parser);

	return (positive ? value : -value);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
uint64_t ParserTraits<Parser, uint64_t>::parse(Parser &parser)
{
	parser.skipWhiteSpace();

	if (parser.currentCharacter() == '-')
		throw ParserException(parser.coordinate(), "expected unsigned integer, got signed one");

	return parseIntegerBody(parser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
int32_t ParserTraits<Parser, int32_t>::parse(Parser &parser)
{
	return static_cast<int32_t>(ParserTraits<Parser, int64_t>::parse(parser));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
uint32_t ParserTraits<Parser, uint32_t>::parse(Parser &parser)
{
	return static_cast<uint32_t>(ParserTraits<Parser, uint64_t>::parse(parser));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, bool>::parse(Parser &parser)
{
	parser.skipWhiteSpace();

	if (parser.template testAndSkip<char>('0'))
	    return false;

	if (parser.template testAndSkip<char>('1'))
		return true;

	throw ParserException(parser.coordinate(), "could not parse Boolean value");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, std::string>::test(Parser &parser, const std::string &expectedValue)
{
	if (!parser.isWhiteSpace(expectedValue.front()))
		parser.skipWhiteSpace();

	const auto match = std::find_if(expectedValue.cbegin(), expectedValue.cend(),
		[&parser](const auto &expectedCharacter)
		{
			const auto character = static_cast<char>(parser.currentCharacter());

			if (character != expectedCharacter)
				return true;

			parser.advance();

			return false;
		});

	return (match == expectedValue.cend());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, char>::test(Parser &parser, const char &expectedValue)
{
	const auto result = (parser.currentCharacter() == expectedValue);

	parser.advance();

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, int64_t>::test(Parser &parser, const int64_t &expectedValue)
{
	const auto value = parse(parser);

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, uint64_t>::test(Parser &parser, const uint64_t &expectedValue)
{
	const auto value = parse(parser);

	return (value == expectedValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, int32_t>::test(Parser &parser, const int32_t &expectedValue)
{
	return ParserTraits<Parser, int64_t>::test(parser, static_cast<int64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Parser>
bool ParserTraits<Parser, uint32_t>::test(Parser &parser, const uint32_t &expectedValue)
{
	return ParserTraits<Parser, uint64_t>::test(parser, static_cast<uint64_t>(expectedValue));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
*/
}
}

#endif
