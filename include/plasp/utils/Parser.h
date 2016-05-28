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

		template<typename Type>
		Type parse();

		template<class CharacterCondition>
		void parseUntil(std::vector<std::string> &container, CharacterCondition characterCondition);

		template<typename Type>
		void expect(const Type &expectedValue);

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

template<class CharacterCondition>
void Parser::parseUntil(std::vector<std::string> &container, CharacterCondition characterCondition)
{
	while (true)
	{
		skipWhiteSpace();

		std::string value;

		while (true)
		{
			const auto character = *m_position;

			if (characterCondition(character))
			{
				container.emplace_back(std::move(value));
				return;
			}

			if (std::isspace(character))
				break;

			value.push_back(character);
			advance();
		}

		container.emplace_back(std::move(value));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
