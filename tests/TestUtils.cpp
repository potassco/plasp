#include <gtest/gtest.h>

#include <plasp/utils/IO.h>
#include <plasp/utils/Parser.h>
#include <plasp/utils/ParserException.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseSimple)
{
	std::stringstream s("identifier  5   \n-51\t 0 1 expected unexpected");
	plasp::utils::Parser p("input", s);

	ASSERT_EQ(p.parse<std::string>(), "identifier");
	ASSERT_EQ(p.parse<size_t>(), 5u);
	ASSERT_EQ(p.parse<int>(), -51);
	ASSERT_EQ(p.parse<bool>(), false);
	ASSERT_EQ(p.parse<bool>(), true);

	ASSERT_NO_THROW(p.expect<std::string>("expected"));
	ASSERT_THROW(p.expect<std::string>("expected"), plasp::utils::ParserException);

	// TODO: test case-insensitive input
	// TODO: test probing
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseUnsignedNumbers)
{
	std::stringstream s("100 200 -300 -400");
	plasp::utils::Parser p("input", s);

	ASSERT_EQ(p.parse<int>(), 100);
	ASSERT_EQ(p.parse<size_t>(), 200u);
	ASSERT_EQ(p.parse<int>(), -300);
	ASSERT_THROW(p.parse<size_t>(), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseEndOfFile)
{
	std::stringstream s1("test");
	plasp::utils::Parser p1("input", s1);

	ASSERT_NO_THROW(p1.expect<std::string>("test"));
	ASSERT_THROW(p1.parse<std::string>(), plasp::utils::ParserException);

	std::stringstream s2("test1 test2 test3");
	plasp::utils::Parser p2("input", s2);

	ASSERT_NO_THROW(p2.expect<std::string>("test1"));
	ASSERT_NO_THROW(p2.expect<std::string>("test2"));
	ASSERT_NO_THROW(p2.expect<std::string>("test3"));
	ASSERT_THROW(p2.parse<std::string>(), plasp::utils::ParserException);

	std::stringstream s3("-127");
	plasp::utils::Parser p3("input", s3);

	p3.expect<int>(-127);
	ASSERT_THROW(p3.parse<int>(), plasp::utils::ParserException);

	std::stringstream s4("128 -1023 -4095");
	plasp::utils::Parser p4("input", s4);

	ASSERT_NO_THROW(p4.expect<size_t>(128));
	ASSERT_NO_THROW(p4.expect<int>(-1023));
	ASSERT_NO_THROW(p4.expect<int>(-4095));
	ASSERT_THROW(p4.parse<int>(), plasp::utils::ParserException);

	std::stringstream s5("0");
	plasp::utils::Parser p5("input", s5);

	p5.expect<bool>(false);
	ASSERT_THROW(p5.parse<bool>(), plasp::utils::ParserException);

	std::stringstream s6("0 1 0");
	plasp::utils::Parser p6("input", s6);

	ASSERT_NO_THROW(p6.expect<bool>(false));
	ASSERT_NO_THROW(p6.expect<bool>(true));
	ASSERT_NO_THROW(p6.expect<bool>(false));
	ASSERT_THROW(p6.parse<bool>(), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParserPosition)
{
	std::stringstream s("123 \n4\ntest1\n test2\ntest3 \ntest4\n\n\n\n");
	plasp::utils::Parser p("input", s);

	const auto startPosition = p.position();

	plasp::utils::Parser::Coordinate c;

	c = p.coordinate();
	ASSERT_EQ(c.row, 1u);
	ASSERT_EQ(c.column, 1u);
	ASSERT_EQ(p.currentCharacter(), '1');

	ASSERT_NO_THROW(p.advance());

	c = p.coordinate();
	ASSERT_EQ(c.row, 1u);
	ASSERT_EQ(c.column, 2u);
	ASSERT_EQ(p.currentCharacter(), '2');

	ASSERT_NO_THROW(p.advance());

	c = p.coordinate();
	ASSERT_EQ(c.row, 1u);
	ASSERT_EQ(c.column, 3u);
	ASSERT_EQ(p.currentCharacter(), '3');

	ASSERT_NO_THROW(p.advance());

	c = p.coordinate();
	ASSERT_EQ(c.row, 1u);
	ASSERT_EQ(c.column, 4u);
	ASSERT_EQ(p.currentCharacter(), ' ');

	ASSERT_NO_THROW(p.advance());

	c = p.coordinate();
	ASSERT_EQ(c.row, 1u);
	ASSERT_EQ(c.column, 5u);
	ASSERT_EQ(p.currentCharacter(), '\n');

	ASSERT_NO_THROW(p.advance());

	c = p.coordinate();
	ASSERT_EQ(c.row, 2u);
	ASSERT_EQ(c.column, 1u);
	ASSERT_EQ(p.currentCharacter(), '4');

	ASSERT_NO_THROW(p.advance());

	ASSERT_NO_THROW(p.expect<std::string>("test1"));

	c = p.coordinate();
	ASSERT_EQ(c.row, 3u);
	ASSERT_EQ(c.column, 6u);

	ASSERT_NO_THROW(p.expect<std::string>("test2"));

	c = p.coordinate();
	ASSERT_EQ(c.row, 4u);
	ASSERT_EQ(c.column, 7u);

	ASSERT_NO_THROW(p.expect<std::string>("test3"));

	c = p.coordinate();
	ASSERT_EQ(c.row, 5u);
	ASSERT_EQ(c.column, 6u);

	ASSERT_NO_THROW(p.skipLine());

	c = p.coordinate();
	ASSERT_EQ(c.row, 6u);
	ASSERT_EQ(c.column, 1u);

	ASSERT_NO_THROW(p.skipLine());

	c = p.coordinate();
	ASSERT_EQ(c.row, 7u);
	ASSERT_EQ(c.column, 1u);

	ASSERT_NO_THROW(p.skipWhiteSpace());

	c = p.coordinate();
	ASSERT_EQ(c.row, 10u);
	ASSERT_EQ(c.column, 1u);
	ASSERT_TRUE(p.atEndOfStream());

	p.reset();
	ASSERT_EQ(p.position(), startPosition);
	ASSERT_FALSE(p.atEndOfStream());

	// TODO: test parser with multiple sections
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParserRemoveComments)
{
	std::stringstream s1("; comment at beginning\ntest1; comment in between\ntest2; comment at end");
	plasp::utils::Parser p1("input", s1);

	p1.removeComments(";", "\n", false);

	plasp::utils::Parser::Coordinate c;

	ASSERT_NO_THROW(p1.expect<std::string>("test1"));

	c = p1.coordinate();
	ASSERT_EQ(c.row, 2u);
	ASSERT_EQ(c.column, 6u);

	ASSERT_NO_THROW(p1.expect<std::string>("test2"));

	c = p1.coordinate();
	ASSERT_EQ(c.row, 3u);
	ASSERT_EQ(c.column, 6u);

	p1.skipWhiteSpace();

	ASSERT_TRUE(p1.atEndOfStream());

	std::stringstream s2("test;");
	plasp::utils::Parser p2("input", s2);

	p2.removeComments(";", "\n", false);

	ASSERT_NO_THROW(p2.expect<std::string>("test"));

	p2.skipWhiteSpace();

	ASSERT_TRUE(p2.atEndOfStream());

	std::stringstream s3("/* comment at start */ test1 /* comment in between */ test2 /*");
	plasp::utils::Parser p3("input", s3);

	p3.removeComments("/*", "*/", true);

	ASSERT_NO_THROW(p3.expect<std::string>("test1"));
	ASSERT_NO_THROW(p3.expect<std::string>("test2"));

	p3.skipWhiteSpace();

	ASSERT_TRUE(p3.atEndOfStream());
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
