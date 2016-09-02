#include <gtest/gtest.h>

#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Not.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DoubleNegation)
{
	auto n1 = std::make_unique<expressions::Not>();
	auto n2 = std::make_unique<expressions::Not>();
	auto d = std::make_unique<expressions::Dummy>();
	const auto dp = d.get();

	n2->setArgument(std::move(d));
	n1->setArgument(std::move(n2));

	auto normalized = n1->normalize();

	ASSERT_EQ(normalized.get(), dp);
	ASSERT_TRUE(dp->isNormalized());
}
