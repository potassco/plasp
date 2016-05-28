#ifndef __PLASP__UTILS__PARSER_H
#define __PLASP__UTILS__PARSER_H

#include <iostream>
#include <iterator>

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

}
}

#endif