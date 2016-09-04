#include <gtest/gtest.h>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Implication)
{
	auto i = expressions::ImplyPointer(new expressions::Imply);
	auto d1 = expressions::DummyPointer(new expressions::Dummy);
	const auto d1p = d1.get();
	auto d2 = expressions::DummyPointer(new expressions::Dummy);
	const auto d2p = d2.get();

	i->setArgument(0, d1);
	i->setArgument(1, d2);

	auto normalized = i->normalized();

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
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);
	auto d = expressions::DummyPointer(new expressions::Dummy);
	const auto dp = d.get();

	n2->setArgument(std::move(d));
	n1->setArgument(std::move(n2));

	auto normalized = n1->normalized();

	ASSERT_EQ(normalized.get(), dp);
	ASSERT_TRUE(dp->isNormalized());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DeMorganNegativeConjunction)
{
	auto d1 = expressions::DummyPointer(new expressions::Dummy);
	const auto d1p = d1.get();
	auto d2 = expressions::DummyPointer(new expressions::Dummy);
	const auto d2p = d2.get();
	auto d3 = expressions::DummyPointer(new expressions::Dummy);
	const auto d3p = d3.get();

	auto a = expressions::AndPointer(new expressions::And);
	a->addArgument(d1);
	a->addArgument(d2);
	a->addArgument(d3);

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(a);

	auto normalized = n->normalized();

	ASSERT_EQ(normalized->expressionType(), Expression::Type::Or);

	const auto &o = dynamic_cast<expressions::Or &>(*normalized);

	ASSERT_EQ(o.arguments()[0]->expressionType(), Expression::Type::Not);
	ASSERT_EQ(o.arguments()[1]->expressionType(), Expression::Type::Not);
	ASSERT_EQ(o.arguments()[2]->expressionType(), Expression::Type::Not);

	const auto &n1 = dynamic_cast<expressions::Not &>(*o.arguments()[0]);
	const auto &n2 = dynamic_cast<expressions::Not &>(*o.arguments()[1]);
	const auto &n3 = dynamic_cast<expressions::Not &>(*o.arguments()[2]);

	ASSERT_EQ(n1.argument().get(), d1p);
	ASSERT_EQ(n2.argument().get(), d2p);
	ASSERT_EQ(n3.argument().get(), d3p);
	ASSERT_TRUE(d1p->isNormalized());
	ASSERT_TRUE(d2p->isNormalized());
	ASSERT_TRUE(d3p->isNormalized());
}


////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DeMorganNegativeDisjunction)
{
	auto d1 = expressions::DummyPointer(new expressions::Dummy);
	const auto d1p = d1.get();
	auto d2 = expressions::DummyPointer(new expressions::Dummy);
	const auto d2p = d2.get();
	auto d3 = expressions::DummyPointer(new expressions::Dummy);
	const auto d3p = d3.get();

	auto o = expressions::OrPointer(new expressions::Or);
	o->addArgument(d1);
	o->addArgument(d2);
	o->addArgument(d3);

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(o);

	auto normalized = n->normalized();

	ASSERT_EQ(normalized->expressionType(), Expression::Type::And);

	const auto &a = dynamic_cast<expressions::And &>(*normalized);

	ASSERT_EQ(a.arguments()[0]->expressionType(), Expression::Type::Not);
	ASSERT_EQ(a.arguments()[1]->expressionType(), Expression::Type::Not);
	ASSERT_EQ(a.arguments()[2]->expressionType(), Expression::Type::Not);

	const auto &n1 = dynamic_cast<expressions::Not &>(*a.arguments()[0]);
	const auto &n2 = dynamic_cast<expressions::Not &>(*a.arguments()[1]);
	const auto &n3 = dynamic_cast<expressions::Not &>(*a.arguments()[2]);

	ASSERT_EQ(n1.argument().get(), d1p);
	ASSERT_EQ(n2.argument().get(), d2p);
	ASSERT_EQ(n3.argument().get(), d3p);
	ASSERT_TRUE(d1p->isNormalized());
	ASSERT_TRUE(d2p->isNormalized());
	ASSERT_TRUE(d3p->isNormalized());
}
