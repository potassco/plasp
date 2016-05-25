#include <gtest/gtest.h>

#include <plasp/utils/Parsing.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(UtilsTests, ParseSimple)
{
	std::stringstream stream("identifier  5   \n-51\t expected unexpected 100 -100");

	ASSERT_EQ(plasp::utils::parse<std::string>(stream), "identifier");
	ASSERT_EQ(plasp::utils::parse<size_t>(stream), 5u);
	ASSERT_EQ(plasp::utils::parse<int>(stream), -51);
	ASSERT_NO_THROW(plasp::utils::parseExpected<std::string>(stream, "expected"));
	ASSERT_THROW(plasp::utils::parseExpected<std::string>(stream, "expected"), plasp::utils::ParserException);
	ASSERT_NO_THROW(plasp::utils::parseExpected<size_t>(stream, 100));
	ASSERT_THROW(plasp::utils::parseExpected<size_t>(stream, 100), plasp::utils::ParserException);
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
