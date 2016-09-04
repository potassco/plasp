#include <gtest/gtest.h>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Simplify)
{
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);
	auto a = expressions::AndPointer(new expressions::And);
	auto o = expressions::OrPointer(new expressions::Or);
	auto i = expressions::ImplyPointer(new expressions::Imply);

	i->setArgument(0, new expressions::Dummy("a"));
	i->setArgument(1, new expressions::Dummy("b"));

	o->addArgument(i);
	o->addArgument(new expressions::Dummy("c"));

	a->addArgument(o);
	a->addArgument(new expressions::Dummy("d"));

	n2->setArgument(a);
	n1->setArgument(n2);

	std::stringstream output;
	n1->simplified()->print(output);

	ASSERT_EQ(output.str(), "(not (not (and (or (or (not (a)) (b)) (c)) (d))))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Implication)
{
	auto i = expressions::ImplyPointer(new expressions::Imply);

	i->setArgument(0, new expressions::Dummy("a"));
	i->setArgument(1, new expressions::Dummy("b"));

	std::stringstream output;
	i->normalized()->print(output);

	ASSERT_EQ(output.str(), "(or (not (a)) (b))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DoubleNegation)
{
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);

	n2->setArgument(new expressions::Dummy("a"));
	n1->setArgument(n2);

	std::stringstream output;
	n1->normalized()->print(output);

	ASSERT_EQ(output.str(), "(a)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DeMorganNegativeConjunction)
{
	auto a = expressions::AndPointer(new expressions::And);
	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(new expressions::Dummy("c"));

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(a);

	std::stringstream output;
	n->normalized()->print(output);

	ASSERT_EQ(output.str(), "(or (not (a)) (not (b)) (not (c)))");
}


////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DeMorganNegativeDisjunction)
{
	auto a = expressions::OrPointer(new expressions::Or);
	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(new expressions::Dummy("c"));

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(a);

	std::stringstream output;
	n->normalized()->print(output);

	ASSERT_EQ(output.str(), "(and (not (a)) (not (b)) (not (c)))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, DoubleNegationInner)
{
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);
	auto n3 = expressions::NotPointer(new expressions::Not);
	auto a = expressions::AndPointer(new expressions::And);

	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(new expressions::Dummy("c"));

	n3->setArgument(a);
	n2->setArgument(n3);
	n1->setArgument(n2);

	std::stringstream output;
	n1->normalized()->print(output);

	ASSERT_EQ(output.str(), "(or (not (a)) (not (b)) (not (c)))");
}
