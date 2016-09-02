#include <gtest/gtest.h>

#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Unsupported.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DoubleNegation)
{
	auto n1 = std::make_unique<expressions::Not>();
	auto n2 = std::make_unique<expressions::Not>();
	auto u = std::make_unique<expressions::Unsupported>();
	const auto up = u.get();

	n2->setArgument(std::move(u));
	n1->setArgument(std::move(n2));

	auto normalized = n1->normalize();

	ASSERT_EQ(normalized.get(), up);
}
