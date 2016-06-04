#ifndef __PLASP__UTILS__PARSER_H
#define __PLASP__UTILS__PARSER_H

#include <iostream>
#include <iterator>
#include <vector>

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
		explicit Parser(std::istream &istream);

		size_t row() const;
		size_t column() const;

		void setCaseSensitive(bool isCaseInsensitive = true);

		char currentCharacter() const;
		void advance();
		bool advanceIf(char expectedCharacter);
		bool atEndOfFile() const;

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

		std::istream &m_istream;
		std::istreambuf_iterator<char> m_position;

		size_t m_row;
		size_t m_column;

		bool m_isCaseSensitive;

		bool m_endOfFile;
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

	while (!atEndOfFile() && whiteSpacePredicate(currentCharacter()))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
