#include <catch.hpp>

#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Dummy.h>
#include <plasp/pddl/expressions/Imply.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Or.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] PDDL expressions are correctly reduced", "[PDDL normalization]")
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

	CHECK(output.str() == "(not (not (and (or (or (not (a)) (b)) (c)) (d))))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] PDDL expressions are correctly simplified", "[PDDL normalization]")
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

	CHECK(output.str() == "(and (a) (b) (c) (d) (or (e) (f) (g) (h)))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Implications are correctly replaced", "[PDDL normalization]")
{
	auto i = expressions::ImplyPointer(new expressions::Imply);

	i->setArgument(0, new expressions::Dummy("a"));
	i->setArgument(1, new expressions::Dummy("b"));

	std::stringstream output;
	i->normalized()->print(output);

	CHECK(output.str() == "(or (not (a)) (b))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Double negations are correctly replaced", "[PDDL normalization]")
{
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);

	n2->setArgument(new expressions::Dummy("a"));
	n1->setArgument(n2);

	std::stringstream output;
	n1->normalized()->print(output);

	CHECK(output.str() == "(a)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] De Morgan’s rule is correctly applied to negative conjunctions", "[PDDL normalization]")
{
	auto a = expressions::AndPointer(new expressions::And);
	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(new expressions::Dummy("c"));

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(a);

	std::stringstream output;
	n->normalized()->print(output);

	CHECK(output.str() == "(or (not (a)) (not (b)) (not (c)))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] De Morgan’s rule is correctly applied to negative disjunctions", "[PDDL normalization]")
{
	auto a = expressions::OrPointer(new expressions::Or);
	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(new expressions::Dummy("c"));

	auto n = expressions::NotPointer(new expressions::Not);
	n->setArgument(a);

	std::stringstream output;
	n->normalized()->print(output);

	CHECK(output.str() == "(and (not (a)) (not (b)) (not (c)))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Expressions inside double negations are also normalized", "[PDDL normalization]")
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

	CHECK(output.str() == "(or (not (a)) (not (b)) (not (c)))");
}
