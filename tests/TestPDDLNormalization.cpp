#include <catch.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////

/*TEST_CASE("[PDDL normalization] PDDL expressions are correctly reduced", "[PDDL normalization]")
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
	i->reduced()->print(output);

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
	n1->simplified()->print(output);

	CHECK(output.str() == "(a)");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] Expressions inside double negations are also simplified", "[PDDL normalization]")
{
	auto n1 = expressions::NotPointer(new expressions::Not);
	auto n2 = expressions::NotPointer(new expressions::Not);
	auto a1 = expressions::AndPointer(new expressions::And);
	auto a2 = expressions::AndPointer(new expressions::And);

	a2->addArgument(new expressions::Dummy("d"));
	a2->addArgument(new expressions::Dummy("e"));

	a1->addArgument(new expressions::Dummy("a"));
	a1->addArgument(new expressions::Dummy("b"));
	a1->addArgument(new expressions::Dummy("c"));
	a1->addArgument(a2);

	n2->setArgument(a1);
	n1->setArgument(n2);

	std::stringstream output;
	n1->simplified()->print(output);

	CHECK(output.str() == "(and (a) (b) (c) (d) (e))");
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
	f1->simplified()->print(output);

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
	e1->simplified()->print(output);

	CHECK(output.str() == "(exists (?x ?y ?z ?u ?v ?w) (a))");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL normalization] “for all” expressions are correctly replaced by “exists” expressions during normalization", "[PDDL normalization]")
{
	auto v1 = expressions::VariablePointer(new expressions::Variable("x"));
	auto v2 = expressions::VariablePointer(new expressions::Variable("y"));
	auto v3 = expressions::VariablePointer(new expressions::Variable("z"));
	auto v4 = expressions::VariablePointer(new expressions::Variable("u"));
	auto v5 = expressions::VariablePointer(new expressions::Variable("v"));
	auto v6 = expressions::VariablePointer(new expressions::Variable("w"));

	auto f1 = expressions::ForAllPointer(new expressions::ForAll);
	auto e = expressions::ExistsPointer(new expressions::Exists);
	auto f2 = expressions::ForAllPointer(new expressions::ForAll);

	f1->variables() = {v1, v2};
	e->variables() = {v3, v4};
	f2->variables() = {v5, v6};

	f1->setArgument(e);
	e->setArgument(f2);
	f2->setArgument(new expressions::Dummy("a"));

	std::stringstream output;
	f1->normalized()->print(output);

	CHECK(output.str() == "(exists (?x ?y) (not (exists (?z ?u ?v ?w) (not (a)))))");
}*/
