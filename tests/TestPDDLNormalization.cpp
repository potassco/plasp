#include <gtest/gtest.h>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Exists.h>
#include <plasp/pddl/expressions/ForAll.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>
#include <plasp/pddl/expressions/Variable.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Reduce)
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
	n1->reduced()->print(output);

	ASSERT_EQ(output.str(), "(not (not (and (or (or (not (a)) (b)) (c)) (d))))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Simplify)
{
	auto a1 = expressions::AndPointer(new expressions::And);
	auto a2 = expressions::AndPointer(new expressions::And);
	auto o1 = expressions::OrPointer(new expressions::Or);
	auto o2 = expressions::OrPointer(new expressions::Or);

	a1->addArgument(new expressions::Dummy("a"));
	a1->addArgument(new expressions::Dummy("b"));
	a1->addArgument(a2);

	a2->addArgument(new expressions::Dummy("c"));
	a2->addArgument(new expressions::Dummy("d"));
	a2->addArgument(o1);

	o1->addArgument(new expressions::Dummy("e"));
	o1->addArgument(new expressions::Dummy("f"));
	o1->addArgument(o2);

	o2->addArgument(new expressions::Dummy("g"));
	o2->addArgument(new expressions::Dummy("h"));

	std::stringstream output;
	a1->simplified()->print(output);

	ASSERT_EQ(output.str(), "(and (a) (b) (c) (d) (or (e) (f) (g) (h)))");
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

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, SimplifyNestedForAll)
{
	auto v1 = expressions::VariablePointer(new expressions::Variable("x"));
	auto v2 = expressions::VariablePointer(new expressions::Variable("y"));
	auto v3 = expressions::VariablePointer(new expressions::Variable("z"));
	auto v4 = expressions::VariablePointer(new expressions::Variable("u"));
	auto v5 = expressions::VariablePointer(new expressions::Variable("v"));
	auto v6 = expressions::VariablePointer(new expressions::Variable("w"));

	auto f1 = expressions::ForAllPointer(new expressions::ForAll);
	auto f2 = expressions::ForAllPointer(new expressions::ForAll);

	f1->variables() = {v1, v2, v3};
	f2->variables() = {v4, v5, v6};

	f1->setArgument(f2);
	f2->setArgument(new expressions::Dummy("a"));

	std::stringstream output;
	f1->normalized()->print(output);

	ASSERT_EQ(output.str(), "(forall (?x ?y ?z ?u ?v ?w) (a))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, SimplifyNestedExists)
{
	auto v1 = expressions::VariablePointer(new expressions::Variable("x"));
	auto v2 = expressions::VariablePointer(new expressions::Variable("y"));
	auto v3 = expressions::VariablePointer(new expressions::Variable("z"));
	auto v4 = expressions::VariablePointer(new expressions::Variable("u"));
	auto v5 = expressions::VariablePointer(new expressions::Variable("v"));
	auto v6 = expressions::VariablePointer(new expressions::Variable("w"));

	auto e1 = expressions::ExistsPointer(new expressions::Exists);
	auto e2 = expressions::ExistsPointer(new expressions::Exists);

	e1->variables() = {v1, v2, v3};
	e2->variables() = {v4, v5, v6};

	e1->setArgument(e2);
	e2->setArgument(new expressions::Dummy("a"));

	std::stringstream output;
	e1->normalized()->print(output);

	ASSERT_EQ(output.str(), "(exists (?x ?y ?z ?u ?v ?w) (a))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLNormalizationTests, Prenex)
{
	auto a = expressions::AndPointer(new expressions::And);
	auto f1 = expressions::ForAllPointer(new expressions::ForAll);
	auto o = expressions::OrPointer(new expressions::Or);
	auto e = expressions::ExistsPointer(new expressions::Exists);
	auto f2 = expressions::ForAllPointer(new expressions::ForAll);

	auto v1 = expressions::VariablePointer(new expressions::Variable("x"));
	auto v2 = expressions::VariablePointer(new expressions::Variable("y"));
	auto v3 = expressions::VariablePointer(new expressions::Variable("z"));

	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(f1);
	f1->variables() = {v1};
	f1->setArgument(o);
	o->addArgument(e);
	e->variables() = {v3};
	e->setArgument(new expressions::Dummy("b"));
	o->addArgument(f2);
	f2->variables() = {v2};
	f2->setArgument(new expressions::Dummy("c"));

	auto normalized = a->reduced()->negationNormalized()->prenex();

	std::stringstream output;
	normalized->print(output);

	ASSERT_EQ(output.str(), "(forall (?x) (exists (?z) (forall (?y) (and (a) (or (b) (c))))))");
}
