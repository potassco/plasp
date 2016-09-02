#include <gtest/gtest.h>

#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Implication)
{
	auto i = std::make_unique<expressions::Imply>();
	auto d1 = std::make_unique<expressions::Dummy>();
	const auto d1p = d1.get();
	auto d2 = std::make_unique<expressions::Dummy>();
	const auto d2p = d2.get();

	i->setArgument<0>(std::move(d1));
	i->setArgument<1>(std::move(d2));

	auto normalized = i->normalize();

	ASSERT_EQ(normalized->expressionType(), Expression::Type::Or);

	const auto &o = dynamic_cast<expressions::Or &>(*normalized);

	ASSERT_EQ(o.arguments()[0]->expressionType(), Expression::Type::Not);

	const auto &n = dynamic_cast<const expressions::Not &>(*o.arguments()[0]);

	ASSERT_EQ(n.argument(), d1p);
	ASSERT_EQ(o.arguments()[1], d2p);
	ASSERT_TRUE(d1p->isNormalized());
	ASSERT_TRUE(d2p->isNormalized());
}

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
