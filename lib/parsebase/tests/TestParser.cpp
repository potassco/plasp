#include <catch.hpp>

#include <parsebase/Parser.h>
#include <parsebase/ParserException.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] Simple strings are parsed correctly", "[parser]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1 100 200 -300 -400");
	parsebase::Parser<> p("input", s);

	REQUIRE(p.parse<std::string>() == "identifier");
	REQUIRE(p.parse<size_t>() == 5u);
	REQUIRE(p.parse<int>() == -51);
	REQUIRE(p.parse<bool>() == false);
	REQUIRE(p.parse<bool>() == true);

	REQUIRE(p.parse<int>() == 100);
	REQUIRE(p.parse<size_t>() == 200u);
	REQUIRE(p.parse<int>() == -300);
	REQUIRE_THROWS_AS(p.parse<size_t>(), parsebase::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] Parsing exceptions are correctly reported", "[parser]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1 100 200 -300 -400");
	parsebase::Parser<> p("input", s);

	REQUIRE_NOTHROW(p.expect<std::string>("identifier"));
	REQUIRE_NOTHROW(p.expect<size_t>(5u));
	REQUIRE_NOTHROW(p.expect<int>(-51));
	REQUIRE_NOTHROW(p.expect<bool>(false));
	REQUIRE_NOTHROW(p.expect<bool>(true));

	REQUIRE_NOTHROW(p.expect<int>(100));
	REQUIRE_NOTHROW(p.expect<size_t>(200u));
	REQUIRE_NOTHROW(p.expect<int>(-300));
	REQUIRE_THROWS_AS(p.expect<size_t>(-400), parsebase::ParserException);

	p.seek(0);
	REQUIRE_THROWS_AS(p.expect<std::string>("error"), parsebase::ParserException);

	p.seek(14);
	REQUIRE_THROWS_AS(p.expect<size_t>(6u), parsebase::ParserException);

	p.seek(17);
	REQUIRE_THROWS_AS(p.expect<int>(-50), parsebase::ParserException);

	p.seek(24);
	REQUIRE_THROWS_AS(p.expect<bool>(true), parsebase::ParserException);

	p.seek(26);
	REQUIRE_THROWS_AS(p.expect<bool>(false), parsebase::ParserException);

	p.seek(28);
	REQUIRE_THROWS_AS(p.expect<int>(101), parsebase::ParserException);

	p.seek(31);
	REQUIRE_THROWS_AS(p.expect<size_t>(201), parsebase::ParserException);

	p.seek(34);
	REQUIRE_THROWS_AS(p.expect<int>(-299), parsebase::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] While parsing, the cursor position is as expected", "[parser]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1");
	parsebase::Parser<> p("input", s);

	parsebase::Parser<>::Position pos;

	pos = p.position();
	REQUIRE(p.testAndReturn<std::string>("error") == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndReturn<std::string>("identifier") == true);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<std::string>("error") == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<std::string>("identifier") == true);
	REQUIRE(p.position() == 12);

	pos = p.position();
	REQUIRE(p.testAndReturn<size_t>(6u) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndReturn<size_t>(5u) == true);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<size_t>(6u) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<size_t>(5u) == true);
	REQUIRE(p.position() == 15);

	pos = p.position();
	REQUIRE(p.testAndReturn<int>(-50) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndReturn<int>(-51) == true);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<int>(-50) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<int>(-51) == true);
	REQUIRE(p.position() == 22);

	pos = p.position();
	REQUIRE(p.testAndReturn<bool>(true) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndReturn<bool>(false) == true);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(true) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(false) == true);
	REQUIRE(p.position() == 25);

	pos = p.position();
	REQUIRE(p.testAndReturn<bool>(false) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndReturn<bool>(true) == true);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(false) == false);
	REQUIRE(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(true) == true);
	REQUIRE(p.position() == 27);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] The end of the input stream is correctly handled", "[parser]")
{
	std::stringstream s1("test");
	parsebase::Parser<> p1("input", s1);

	REQUIRE_NOTHROW(p1.expect<std::string>("test"));
	REQUIRE_THROWS_AS(p1.parse<std::string>(), parsebase::ParserException);

	std::stringstream s2("test1 test2 test3");
	parsebase::Parser<> p2("input", s2);

	REQUIRE_NOTHROW(p2.expect<std::string>("test1"));
	REQUIRE_NOTHROW(p2.expect<std::string>("test2"));
	REQUIRE_NOTHROW(p2.expect<std::string>("test3"));
	REQUIRE_THROWS_AS(p2.parse<std::string>(), parsebase::ParserException);

	std::stringstream s3("-127");
	parsebase::Parser<> p3("input", s3);

	p3.expect<int>(-127);
	REQUIRE_THROWS_AS(p3.parse<int>(), parsebase::ParserException);

	std::stringstream s4("128 -1023 -4095");
	parsebase::Parser<> p4("input", s4);

	REQUIRE_NOTHROW(p4.expect<size_t>(128));
	REQUIRE_NOTHROW(p4.expect<int>(-1023));
	REQUIRE_NOTHROW(p4.expect<int>(-4095));
	REQUIRE_THROWS_AS(p4.parse<int>(), parsebase::ParserException);

	std::stringstream s5("0");
	parsebase::Parser<> p5("input", s5);

	p5.expect<bool>(false);
	REQUIRE_THROWS_AS(p5.parse<bool>(), parsebase::ParserException);

	std::stringstream s6("0 1 0");
	parsebase::Parser<> p6("input", s6);

	REQUIRE_NOTHROW(p6.expect<bool>(false));
	REQUIRE_NOTHROW(p6.expect<bool>(true));
	REQUIRE_NOTHROW(p6.expect<bool>(false));
	REQUIRE_THROWS_AS(p6.parse<bool>(), parsebase::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] While parsing, the cursor location is as expcected", "[parser]")
{
	std::stringstream s("123 \n4\ntest1\n test2\ntest3 \ntest4\n\n\n\n");
	parsebase::Parser<> p("input", s);

	const auto startPosition = p.position();

	parsebase::Location l;

	l = p.location();
	REQUIRE(l.rowStart == 1u);
	REQUIRE(l.columnStart == 1u);
	REQUIRE(p.currentCharacter() == '1');

	REQUIRE_NOTHROW(p.advance());

	l = p.location();
	REQUIRE(l.rowStart == 1u);
	REQUIRE(l.columnStart == 2u);
	REQUIRE(p.currentCharacter() == '2');

	REQUIRE_NOTHROW(p.advance());

	l = p.location();
	REQUIRE(l.rowStart == 1u);
	REQUIRE(l.columnStart == 3u);
	REQUIRE(p.currentCharacter() == '3');

	REQUIRE_NOTHROW(p.advance());

	l = p.location();
	REQUIRE(l.rowStart == 1u);
	REQUIRE(l.columnStart == 4u);
	REQUIRE(p.currentCharacter() == ' ');

	REQUIRE_NOTHROW(p.advance());

	l = p.location();
	REQUIRE(l.rowStart == 1u);
	REQUIRE(l.columnStart == 5u);
	REQUIRE(p.currentCharacter() == '\n');

	REQUIRE_NOTHROW(p.advance());

	l = p.location();
	REQUIRE(l.rowStart == 2u);
	REQUIRE(l.columnStart == 1u);
	REQUIRE(p.currentCharacter() == '4');

	REQUIRE_NOTHROW(p.advance());

	REQUIRE_NOTHROW(p.expect<std::string>("test1"));

	l = p.location();
	REQUIRE(l.rowStart == 3u);
	REQUIRE(l.columnStart == 6u);

	REQUIRE_NOTHROW(p.expect<std::string>("test2"));

	l = p.location();
	REQUIRE(l.rowStart == 4u);
	REQUIRE(l.columnStart == 7u);

	REQUIRE_NOTHROW(p.expect<std::string>("test3"));

	l = p.location();
	REQUIRE(l.rowStart == 5u);
	REQUIRE(l.columnStart == 6u);

	REQUIRE_NOTHROW(p.skipLine());

	l = p.location();
	REQUIRE(l.rowStart == 6u);
	REQUIRE(l.columnStart == 1u);

	REQUIRE_NOTHROW(p.skipLine());

	l = p.location();
	REQUIRE(l.rowStart == 7u);
	REQUIRE(l.columnStart == 1u);

	REQUIRE_NOTHROW(p.skipWhiteSpace());

	l = p.location();
	REQUIRE(l.rowStart == 10u);
	REQUIRE(l.columnStart == 1u);
	REQUIRE(p.atEnd());

	p.reset();
	REQUIRE(p.position() == startPosition);
	REQUIRE_FALSE(p.atEnd());

	for (size_t i = 0; i < 5; i++)
		p.advance();

	REQUIRE(p.position() == static_cast<std::istream::pos_type>(5));

	p.seek(static_cast<std::istream::pos_type>(7));

	REQUIRE(p.position() == static_cast<std::istream::pos_type>(7));

	REQUIRE_NOTHROW(p.expect<std::string>("test1"));

	// TODO: test parser with multiple sections
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] Comments are correctly removed", "[parser]")
{
	std::stringstream s1("; comment at beginning\ntest1; comment in between\ntest2; comment at end");
	parsebase::Parser<> p1("input", s1);

	p1.removeComments(";", "\n", false);

	parsebase::Location l;

	REQUIRE_NOTHROW(p1.expect<std::string>("test1"));

	l = p1.location();
	REQUIRE(l.rowStart == 2u);
	REQUIRE(l.columnStart == 6u);

	REQUIRE_NOTHROW(p1.expect<std::string>("test2"));

	l = p1.location();
	REQUIRE(l.rowStart == 3u);
	REQUIRE(l.columnStart == 6u);

	p1.skipWhiteSpace();

	REQUIRE(p1.atEnd());

	std::stringstream s2("test;");
	parsebase::Parser<> p2("input", s2);

	p2.removeComments(";", "\n", false);

	REQUIRE_NOTHROW(p2.expect<std::string>("test"));

	p2.skipWhiteSpace();

	REQUIRE(p2.atEnd());

	std::stringstream s3("/* comment at start */ test1 /* comment in between */ test2 /*");
	parsebase::Parser<> p3("input", s3);

	p3.removeComments("/*", "*/", true);

	REQUIRE_NOTHROW(p3.expect<std::string>("test1"));
	REQUIRE_NOTHROW(p3.expect<std::string>("test2"));

	p3.skipWhiteSpace();

	REQUIRE(p3.atEnd());
}
