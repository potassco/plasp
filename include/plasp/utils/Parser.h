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
		Parser(std::istream &istream);

		size_t row() const;
		size_t column() const;

		template<typename T>
		T parse();

		template<typename T>
		void expect(const T &expectedValue);

		void skipWhiteSpace();
		void skipLine();

		std::string getLine();

	private:
		static const std::istream_iterator<unsigned char> EndOfFile;

	private:
		void checkStream() const;
		void advance();

		bool advanceIf(unsigned char expectedCharacter);

		uint64_t parseIntegerBody();

		std::istream &m_istream;
		std::istream_iterator<unsigned char> m_position;

		size_t m_row;
		size_t m_column;

		bool m_endOfFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
