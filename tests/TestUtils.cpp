#include <gtest/gtest.h>

#include <plasp/utils/IO.h>
#include <plasp/utils/Parser.h>
#include <plasp/utils/ParserException.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseSimple)
{
	std::stringstream s("identifier  5   \n-51\t 0 1 expected unexpected");
	plasp::utils::Parser p(s);

	ASSERT_EQ(p.parse<std::string>(), "identifier");
	ASSERT_EQ(p.parse<size_t>(), 5u);
	ASSERT_EQ(p.parse<int>(), -51);
	ASSERT_EQ(p.parse<bool>(), false);
	ASSERT_EQ(p.parse<bool>(), true);

	ASSERT_NO_THROW(p.expect<std::string>("expected"));
	ASSERT_THROW(p.expect<std::string>("expected"), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseUnsignedNumbers)
{
	std::stringstream s("100 200 -300 -400");
	plasp::utils::Parser p(s);

	ASSERT_EQ(p.parse<int>(), 100);
	ASSERT_EQ(p.parse<size_t>(), 200u);
	ASSERT_EQ(p.parse<int>(), -300);
	ASSERT_THROW(p.parse<size_t>(), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseEndOfFile)
{
	std::stringstream s1("test");
	plasp::utils::Parser p1(s1);

	ASSERT_NO_THROW(p1.expect<std::string>("test"));
	ASSERT_THROW(p1.parse<std::string>(), plasp::utils::ParserException);

	std::stringstream s2("test1 test2 test3");
	plasp::utils::Parser p2(s2);

	ASSERT_NO_THROW(p2.expect<std::string>("test1"));
	ASSERT_NO_THROW(p2.expect<std::string>("test2"));
	ASSERT_NO_THROW(p2.expect<std::string>("test3"));
	ASSERT_THROW(p2.parse<std::string>(), plasp::utils::ParserException);

	std::stringstream s3("-127");
	plasp::utils::Parser p3(s3);

	p3.expect<int>(-127);
	ASSERT_THROW(p3.parse<int>(), plasp::utils::ParserException);

	std::stringstream s4("128 -1023 -4095");
	plasp::utils::Parser p4(s4);

	ASSERT_NO_THROW(p4.expect<size_t>(128));
	ASSERT_NO_THROW(p4.expect<int>(-1023));
	ASSERT_NO_THROW(p4.expect<int>(-4095));
	ASSERT_THROW(p4.parse<int>(), plasp::utils::ParserException);

	std::stringstream s5("0");
	plasp::utils::Parser p5(s5);

	p5.expect<bool>(false);
	ASSERT_THROW(p5.parse<bool>(), plasp::utils::ParserException);

	std::stringstream s6("0 1 0");
	plasp::utils::Parser p6(s6);

	ASSERT_NO_THROW(p6.expect<bool>(false));
	ASSERT_NO_THROW(p6.expect<bool>(true));
	ASSERT_NO_THROW(p6.expect<bool>(false));
	ASSERT_THROW(p6.parse<bool>(), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParserPosition)
{
	std::stringstream s("123 \n4\ntest1\n test2\ntest3 \n\n\n\n\n");
	plasp::utils::Parser p(s);

	ASSERT_EQ(p.row(), 1u);
	ASSERT_EQ(p.column(), 1u);
	ASSERT_EQ(p.currentCharacter(), '1');

	ASSERT_NO_THROW(p.advance());

	ASSERT_EQ(p.row(), 1u);
	ASSERT_EQ(p.column(), 2u);
	ASSERT_EQ(p.currentCharacter(), '2');

	ASSERT_NO_THROW(p.advance());

	ASSERT_EQ(p.row(), 1u);
	ASSERT_EQ(p.column(), 3u);
	ASSERT_EQ(p.currentCharacter(), '3');

	ASSERT_NO_THROW(p.advance());

	ASSERT_EQ(p.row(), 1u);
	ASSERT_EQ(p.column(), 4u);
	ASSERT_EQ(p.currentCharacter(), ' ');

	ASSERT_NO_THROW(p.advance());

	ASSERT_EQ(p.row(), 1u);
	ASSERT_EQ(p.column(), 5u);
	ASSERT_EQ(p.currentCharacter(), '\n');

	ASSERT_NO_THROW(p.advance());

	ASSERT_EQ(p.row(), 2u);
	ASSERT_EQ(p.column(), 1u);
	ASSERT_EQ(p.currentCharacter(), '4');

	ASSERT_NO_THROW(p.advance());

	ASSERT_NO_THROW(p.expect<std::string>("test1"));

	ASSERT_EQ(p.row(), 3u);
	ASSERT_EQ(p.column(), 6u);

	ASSERT_NO_THROW(p.expect<std::string>("test2"));

	ASSERT_EQ(p.row(), 4u);
	ASSERT_EQ(p.column(), 7u);

	ASSERT_NO_THROW(p.expect<std::string>("test3"));

	ASSERT_EQ(p.row(), 5u);
	ASSERT_EQ(p.column(), 6u);

	ASSERT_NO_THROW(p.skipWhiteSpace());

	ASSERT_TRUE(p.atEndOfFile());
	ASSERT_EQ(p.row(), 10u);
	ASSERT_EQ(p.column(), 1u);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, EscapeASP)
{
	const std::string predicate = "action(stack_on(block-1, block-2, value@3, value@4))";

	const auto escaped = plasp::utils::escapeASP(predicate);
	const auto unescaped = plasp::utils::unescapeASP(escaped);

	ASSERT_EQ(escaped.find("-"), std::string::npos);
	ASSERT_EQ(escaped.find("@"), std::string::npos);
	ASSERT_EQ(predicate, unescaped);
}
