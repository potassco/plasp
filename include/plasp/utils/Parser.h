#ifndef __PLASP__UTILS__PARSER_H
#define __PLASP__UTILS__PARSER_H

#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <boost/filesystem.hpp>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parser
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Parser
{
	public:
		using Position = std::stringstream::pos_type;

		struct Coordinate
		{
			std::string sectionName;
			size_t row;
			size_t column;
		};

		struct StreamDelimiter
		{
			Position position;
			std::string sectionName;
		};

	public:
		explicit Parser();
		explicit Parser(std::string streamName, std::istream &istream);

		void readStream(std::string streamName, std::istream &istream);
		void readFile(const boost::filesystem::path &path);

		void reset();
		void seek(Position position);
		Position position() const;
		Coordinate coordinate() const;

		void setCaseSensitive(bool isCaseInsensitive = true);

		char currentCharacter() const;
		void advance();
		bool advanceIf(char expectedCharacter);
		bool atEndOfStream() const;

		template<typename Type>
		Type parse();

		template<class CharacterPredicate, class WhiteSpacePredicate>
		std::string parseIdentifier(CharacterPredicate characterPredicate, WhiteSpacePredicate whiteSpacePredicate);

		template<class CharacterPredicate>
		std::string parseIdentifier(CharacterPredicate characterPredicate);

		template<typename Type>
		void expect(const Type &expectedValue);

		template<class WhiteSpacePredicate>
		void skipWhiteSpace(WhiteSpacePredicate whiteSpacePredicate);

		void skipWhiteSpace();
		void skipLine();

		std::string getLine();

	private:
		static const std::istreambuf_iterator<char> EndOfFile;

	private:
		void checkStream() const;

		uint64_t parseIntegerBody();

		mutable std::stringstream m_stream;

		std::vector<StreamDelimiter> m_streamDelimiters;

		bool m_isCaseSensitive;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharacterPredicate, class WhiteSpacePredicate>
std::string Parser::parseIdentifier(CharacterPredicate characterPredicate, WhiteSpacePredicate whiteSpacePredicate)
{
	skipWhiteSpace(whiteSpacePredicate);

	std::string value;

	while (true)
	{
		const auto character = currentCharacter();

		if (!characterPredicate(character))
			return value;

		value.push_back(character);
		advance();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class CharacterPredicate>
std::string Parser::parseIdentifier(CharacterPredicate characterPredicate)
{
	return parseIdentifier(characterPredicate,
		[&](const auto character)
		{
			return std::isspace(character);
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class WhiteSpacePredicate>
void Parser::skipWhiteSpace(WhiteSpacePredicate whiteSpacePredicate)
{
	checkStream();

	while (!atEndOfStream() && whiteSpacePredicate(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
