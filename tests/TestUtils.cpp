#include <gtest/gtest.h>

#include <plasp/utils/Parsing.h>

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
