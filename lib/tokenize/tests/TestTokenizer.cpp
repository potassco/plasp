#include <catch.hpp>

#include <tokenize/Tokenizer.h>
#include <tokenize/TokenizerException.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] Simple strings are tokenized correctly", "[tokenizer]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1 100 200 -300 -400");
	tokenize::Tokenizer<> p("input", s);

	REQUIRE(p.get<std::string>() == "identifier");
	REQUIRE(p.get<size_t>() == 5);
	REQUIRE(p.get<int>() == -51);
	REQUIRE(p.get<bool>() == false);
	REQUIRE(p.get<bool>() == true);

	REQUIRE(p.get<int>() == 100);
	REQUIRE(p.get<size_t>() == 200);
	REQUIRE(p.get<int>() == -300);
	REQUIRE_THROWS_AS(p.get<size_t>(), tokenize::TokenizerException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] Tokenizing exceptions are correctly reported", "[tokenizer]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1 100 200 -300 -400");
	tokenize::Tokenizer<> p("input", s);

	REQUIRE_NOTHROW(p.expect<std::string>("identifier"));
	REQUIRE_NOTHROW(p.expect<size_t>(5));
	REQUIRE_NOTHROW(p.expect<int>(-51));
	REQUIRE_NOTHROW(p.expect<bool>(false));
	REQUIRE_NOTHROW(p.expect<bool>(true));

	REQUIRE_NOTHROW(p.expect<int>(100));
	REQUIRE_NOTHROW(p.expect<size_t>(200));
	REQUIRE_NOTHROW(p.expect<int>(-300));
	REQUIRE_THROWS_AS(p.expect<size_t>(-400), tokenize::TokenizerException);

	p.seek(0);
	REQUIRE_THROWS_AS(p.expect<std::string>("error"), tokenize::TokenizerException);

	p.seek(14);
	REQUIRE_THROWS_AS(p.expect<size_t>(6), tokenize::TokenizerException);

	p.seek(17);
	REQUIRE_THROWS_AS(p.expect<int>(-50), tokenize::TokenizerException);

	p.seek(24);
	REQUIRE_THROWS_AS(p.expect<bool>(true), tokenize::TokenizerException);

	p.seek(26);
	REQUIRE_THROWS_AS(p.expect<bool>(false), tokenize::TokenizerException);

	p.seek(28);
	REQUIRE_THROWS_AS(p.expect<int>(101), tokenize::TokenizerException);

	p.seek(31);
	REQUIRE_THROWS_AS(p.expect<size_t>(201), tokenize::TokenizerException);

	p.seek(34);
	REQUIRE_THROWS_AS(p.expect<int>(-299), tokenize::TokenizerException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] While tokenizing, the cursor position is as expected", "[tokenizer]")
{
	std::stringstream s("  identifier  5   \n-51\t 0 1");
	tokenize::Tokenizer<> p("input", s);

	tokenize::StreamPosition pos;

	pos = p.position();
	REQUIRE(p.testAndReturn<std::string>("error") == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndReturn<std::string>("identifier") == true);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<std::string>("error") == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<std::string>("identifier") == true);
	CHECK(p.position() == 12);

	pos = p.position();
	REQUIRE(p.testAndReturn<size_t>(6) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndReturn<size_t>(5) == true);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<size_t>(6) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<size_t>(5) == true);
	CHECK(p.position() == 15);

	pos = p.position();
	REQUIRE(p.testAndReturn<int>(-50) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndReturn<int>(-51) == true);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<int>(-50) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<int>(-51) == true);
	CHECK(p.position() == 22);

	pos = p.position();
	REQUIRE(p.testAndReturn<bool>(true) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndReturn<bool>(false) == true);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(true) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(false) == true);
	CHECK(p.position() == 25);

	pos = p.position();
	REQUIRE(p.testAndReturn<bool>(false) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndReturn<bool>(true) == true);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(false) == false);
	CHECK(p.position() == pos);
	REQUIRE(p.testAndSkip<bool>(true) == true);
	CHECK(p.position() == 27);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] The end of the input stream is correctly handled", "[tokenizer]")
{
	std::stringstream s1("test");
	tokenize::Tokenizer<> p1("input", s1);

	REQUIRE_NOTHROW(p1.expect<std::string>("test"));
	REQUIRE_THROWS_AS(p1.get<std::string>(), tokenize::TokenizerException);

	std::stringstream s2("test1 test2 test3");
	tokenize::Tokenizer<> p2("input", s2);

	REQUIRE_NOTHROW(p2.expect<std::string>("test1"));
	REQUIRE_NOTHROW(p2.expect<std::string>("test2"));
	REQUIRE_NOTHROW(p2.expect<std::string>("test3"));
	REQUIRE_THROWS_AS(p2.get<std::string>(), tokenize::TokenizerException);

	std::stringstream s3("-127");
	tokenize::Tokenizer<> p3("input", s3);

	p3.expect<int>(-127);
	REQUIRE_THROWS_AS(p3.get<int>(), tokenize::TokenizerException);

	std::stringstream s4("128 -1023 -4095");
	tokenize::Tokenizer<> p4("input", s4);

	REQUIRE_NOTHROW(p4.expect<size_t>(128));
	REQUIRE_NOTHROW(p4.expect<int>(-1023));
	REQUIRE_NOTHROW(p4.expect<int>(-4095));
	REQUIRE_THROWS_AS(p4.get<int>(), tokenize::TokenizerException);

	std::stringstream s5("0");
	tokenize::Tokenizer<> p5("input", s5);

	p5.expect<bool>(false);
	REQUIRE_THROWS_AS(p5.get<bool>(), tokenize::TokenizerException);

	std::stringstream s6("0 1 0");
	tokenize::Tokenizer<> p6("input", s6);

	REQUIRE_NOTHROW(p6.expect<bool>(false));
	REQUIRE_NOTHROW(p6.expect<bool>(true));
	REQUIRE_NOTHROW(p6.expect<bool>(false));
	REQUIRE_THROWS_AS(p6.get<bool>(), tokenize::TokenizerException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] While tokenizing, the cursor location is as expcected", "[tokenizer]")
{
	std::stringstream s("123 \n4\ntest1\n test2\ntest3 \ntest4\n\n\n\n");
	tokenize::Tokenizer<> p("input", s);

	const auto startPosition = p.position();

	tokenize::Location l;

	l = p.location();
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '1');

	p.advance();

	l = p.location();
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 2);
	CHECK(p.currentCharacter() == '2');

	p.advance();

	l = p.location();
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 3);
	CHECK(p.currentCharacter() == '3');

	p.advance();

	l = p.location();
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 4);
	CHECK(p.currentCharacter() == ' ');

	p.advance();

	l = p.location();
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 5);
	CHECK(p.currentCharacter() == '\n');

	p.advance();

	l = p.location();
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '4');

	p.advance();

	REQUIRE_NOTHROW(p.expect<std::string>("test1"));

	l = p.location();
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 6);

	REQUIRE_NOTHROW(p.expect<std::string>("test2"));

	l = p.location();
	CHECK(l.rowStart == 4);
	CHECK(l.columnStart == 7);

	REQUIRE_NOTHROW(p.expect<std::string>("test3"));

	l = p.location();
	CHECK(l.rowStart == 5);
	CHECK(l.columnStart == 6);

	REQUIRE_NOTHROW(p.skipLine());

	l = p.location();
	CHECK(l.rowStart == 6);
	CHECK(l.columnStart == 1);

	REQUIRE_NOTHROW(p.skipLine());

	l = p.location();
	CHECK(l.rowStart == 7);
	CHECK(l.columnStart == 1);

	REQUIRE_NOTHROW(p.skipWhiteSpace());

	l = p.location();
	CHECK(l.rowStart == 10);
	CHECK(l.columnStart == 1);
	CHECK(p.atEnd());

	p.reset();
	CHECK(p.position() == startPosition);
	CHECK_FALSE(p.atEnd());

	for (size_t i = 0; i < 5; i++)
		p.advance();

	CHECK(p.position() == static_cast<std::istream::pos_type>(5));

	p.seek(static_cast<std::istream::pos_type>(7));

	CHECK(p.position() == static_cast<std::istream::pos_type>(7));

	REQUIRE_NOTHROW(p.expect<std::string>("test1"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] While tokenizing with multiple sections, the cursor location is as expcected", "[tokenizer]")
{
	std::stringstream s1("123 \n4\ntest1\n");
	std::stringstream s2("456 \n7\ntest2\n");
	tokenize::Tokenizer<> p;
	p.read("test-1", s1);
	p.read("test-2", s2);

	const auto advance =
		[&](auto steps)
		{
			for (auto i = 0; i < steps; i++)
				p.advance();
		};

	tokenize::Location l;

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '1');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 2);
	CHECK(p.currentCharacter() == '2');

	advance(3);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 5);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '4');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 2);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == 't');

	advance(4);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 5);
	CHECK(p.currentCharacter() == '1');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-1");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 6);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '4');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 2);
	CHECK(p.currentCharacter() == '5');

	advance(3);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 1);
	CHECK(l.columnStart == 5);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == '7');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 2);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 1);
	CHECK(p.currentCharacter() == 't');

	advance(4);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 5);
	CHECK(p.currentCharacter() == '2');

	advance(1);

	l = p.location();
	CHECK(l.sectionStart == "test-2");
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 6);
	CHECK(p.currentCharacter() == '\n');

	advance(1);

	CHECK(p.atEnd());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[tokenizer] Comments are correctly removed", "[tokenizer]")
{
	std::stringstream s1("; comment at beginning\ntest1; comment in between\ntest2; comment at end");
	tokenize::Tokenizer<> p1("input", s1);

	p1.removeComments(";", "\n", false);

	REQUIRE_NOTHROW(p1.expect<std::string>("test1"));

	tokenize::Location l;

	l = p1.location();
	CHECK(l.rowStart == 2);
	CHECK(l.columnStart == 6);

	REQUIRE_NOTHROW(p1.expect<std::string>("test2"));

	l = p1.location();
	CHECK(l.rowStart == 3);
	CHECK(l.columnStart == 6);

	p1.skipWhiteSpace();

	CHECK(p1.atEnd());

	std::stringstream s2("test;");
	tokenize::Tokenizer<> p2("input", s2);

	p2.removeComments(";", "\n", false);

	REQUIRE_NOTHROW(p2.expect<std::string>("test"));

	p2.skipWhiteSpace();

	CHECK(p2.atEnd());

	std::stringstream s3("/* comment at start */ test1 /* comment in between */ test2 /*");
	tokenize::Tokenizer<> p3("input", s3);

	p3.removeComments("/*", "*/", true);

	REQUIRE_NOTHROW(p3.expect<std::string>("test1"));
	REQUIRE_NOTHROW(p3.expect<std::string>("test2"));

	p3.skipWhiteSpace();

	CHECK(p3.atEnd());

	// Check that if there are no comments, the end is not accidentally truncated
	std::stringstream s4("test foo bar");
	tokenize::Tokenizer<> p4("input", s4);

	p4.removeComments(";", "\n", false);

	REQUIRE_NOTHROW(p4.expect<std::string>("test"));
	REQUIRE_NOTHROW(p4.expect<std::string>("foo"));
	REQUIRE_NOTHROW(p4.expect<std::string>("bar"));

	CHECK(p4.atEnd());
}
