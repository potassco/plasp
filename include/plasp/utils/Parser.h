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
		using CharacterType = unsigned char;

	public:
		explicit Parser(std::istream &istream);

		size_t row() const;
		size_t column() const;

		CharacterType currentCharacter() const;

		template<typename Type>
		Type parse();

		template<class WhiteSpacePredicate, class CharacterPredicate>
		std::string parseIdentifier(WhiteSpacePredicate whiteSpacePredicate, CharacterPredicate characterPredicate);

		template<typename Type>
		void expect(const Type &expectedValue);

		template<class WhiteSpacePredicate>
		void skipWhiteSpace(WhiteSpacePredicate whiteSpacePredicate);

		void skipWhiteSpace();
		void skipLine();

		std::string getLine();

	private:
		static const std::istream_iterator<CharacterType> EndOfFile;

	private:
		void checkStream() const;
		void advance();

		bool advanceIf(CharacterType expectedCharacter);

		uint64_t parseIntegerBody();

		std::istream &m_istream;
		std::istream_iterator<CharacterType> m_position;

		size_t m_row;
		size_t m_column;

		bool m_endOfFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class WhiteSpacePredicate, class CharacterPredicate>
std::string Parser::parseIdentifier(WhiteSpacePredicate whiteSpacePredicate, CharacterPredicate characterPredicate)
{
	skipWhiteSpace(whiteSpacePredicate);

	std::string value;

	while (true)
	{
		const auto character = *m_position;

		if (!characterPredicate(character))
			return value;

		value.push_back(character);
		advance();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class WhiteSpacePredicate>
void Parser::skipWhiteSpace(WhiteSpacePredicate whiteSpacePredicate)
{
	checkStream();

	while (whiteSpacePredicate(*m_position))
		advance();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
