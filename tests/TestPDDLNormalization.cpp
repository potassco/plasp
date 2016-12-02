#include <catch.hpp>

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

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Nested “for all” expressions are correctly simplified", "[PDDL normalization]")
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

	CHECK(output.str() == "(forall (?x ?y ?z ?u ?v ?w) (a))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Nested “exists” expressions are correctly simplified", "[PDDL normalization]")
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

	CHECK(output.str() == "(exists (?x ?y ?z ?u ?v ?w) (a))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Prenex normal form is correctly established", "[PDDL normalization]")
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

	SECTION("normalized")
	{
		std::stringstream output;
		normalized->print(output);

		CHECK(output.str() == "(forall (?x) (forall (?y) (exists (?z) (and (a) (or (b) (c))))))");
	}

	SECTION("simplified")
	{
		normalized = normalized->simplified();

		std::stringstream output;
		normalized->print(output);

		CHECK(output.str() == "(forall (?x ?y) (exists (?z) (and (a) (or (b) (c)))))");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Same-type first-order expressions are correctly grouped in prenex normal form", "[PDDL normalization]")
{
	auto f1 = expressions::ForAllPointer(new expressions::ForAll);
	auto f2 = expressions::ForAllPointer(new expressions::ForAll);
	auto f3 = expressions::ForAllPointer(new expressions::ForAll);
	auto f4 = expressions::ForAllPointer(new expressions::ForAll);
	auto f5 = expressions::ForAllPointer(new expressions::ForAll);
	auto f6 = expressions::ForAllPointer(new expressions::ForAll);
	auto e1 = expressions::ExistsPointer(new expressions::Exists);
	auto e2 = expressions::ExistsPointer(new expressions::Exists);
	auto e3 = expressions::ExistsPointer(new expressions::Exists);
	auto a = expressions::AndPointer(new expressions::And);

	f1->variables() = {new expressions::Variable("v1")};
	f1->setArgument(a);

	// forall exists forall exists
	a->addArgument(f2);
	f2->variables() = {new expressions::Variable("v2")};
	f2->setArgument(e1);
	e1->variables() = {new expressions::Variable("v3")};
	e1->setArgument(f3);
	f3->variables() = {new expressions::Variable("v4")};
	f3->setArgument(e2);
	e2->variables() = {new expressions::Variable("v5")};
	e2->setArgument(new expressions::Dummy("a"));

	// forall forall exists forall
	a->addArgument(f4);
	f4->variables() = {new expressions::Variable("v6")};
	f4->setArgument(f5);
	f5->variables() = {new expressions::Variable("v7")};
	f5->setArgument(e3);
	e3->variables() = {new expressions::Variable("v8")};
	e3->setArgument(f6);
	f6->variables() = {new expressions::Variable("v9")};
	f6->setArgument(new expressions::Dummy("b"));

	auto normalized = f1->normalized();

	std::stringstream output;
	normalized->print(output);

	CHECK(output.str() == "(forall (?v1 ?v2 ?v6 ?v7) (exists (?v3 ?v8) (forall (?v4 ?v9) (exists (?v5) (and (a) (b))))))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Disjunctive normal form is correctly established", "[PDDL normalization]")
{
	auto f = expressions::ForAllPointer(new expressions::ForAll);
	auto e = expressions::ExistsPointer(new expressions::Exists);
	auto a = expressions::AndPointer(new expressions::And);
	auto o1 = expressions::OrPointer(new expressions::Or);
	auto o2 = expressions::OrPointer(new expressions::Or);
	auto o3 = expressions::OrPointer(new expressions::Or);

	f->variables() = {new expressions::Variable("v1")};
	f->setArgument(e);

	e->variables() = {new expressions::Variable("v2")};
	e->setArgument(o1);

	o1->addArgument(a);
	o1->addArgument(new expressions::Dummy("h"));

	a->addArgument(new expressions::Dummy("a"));
	a->addArgument(new expressions::Dummy("b"));
	a->addArgument(o2);
	a->addArgument(o3);

	o2->addArgument(new expressions::Dummy("c"));
	o2->addArgument(new expressions::Dummy("d"));
	o2->addArgument(new expressions::Dummy("e"));

	o3->addArgument(new expressions::Dummy("f"));
	o3->addArgument(new expressions::Dummy("g"));

	auto normalized = f->normalized();

	std::stringstream output;
	normalized->print(output);

	CHECK(output.str() == "(forall (?v1) (exists (?v2) (or "
		"(and (a) (b) (c) (f)) "
		"(h) "
		"(and (a) (b) (d) (f)) "
		"(and (a) (b) (e) (f)) "
		"(and (a) (b) (c) (g)) "
		"(and (a) (b) (d) (g)) "
		"(and (a) (b) (e) (g))"
		")))");
}
