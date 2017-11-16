#include <catch.hpp>

#include <experimental/filesystem>

#include <pddl/AST.h>
#include <pddl/Parse.h>
#include <pddl/Normalize.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] A simple PDDL description is preserved by normalization", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto domainFile = fs::path("data") / "normalization" / "normalization-1.pddl";
	context.tokenizer.read(domainFile);
	auto description = pddl::parseDescription(context);
	const auto normalizedDescription = pddl::normalize(std::move(description));

	const auto &types = normalizedDescription.domain->types;

	REQUIRE(types.size() == 2);

	CHECK(types[0]->name == "block");
	CHECK(types[1]->name == "gripper");

	const auto &predicates = normalizedDescription.domain->predicates;

	REQUIRE(predicates.size() == 3);

	CHECK(predicates[0]->name == "test-predicate-0");
	CHECK(predicates[0]->parameters.size() == 0);
	CHECK(predicates[2]->name == "test-predicate-2");
	REQUIRE(predicates[2]->parameters.size() == 2);
	CHECK(predicates[2]->parameters[0]->name == "x");
	REQUIRE(predicates[2]->parameters[0]->type);
	CHECK(predicates[2]->parameters[0]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[0].get());
	CHECK(predicates[2]->parameters[1]->name == "y");
	REQUIRE(predicates[2]->parameters[1]->type);
	CHECK(predicates[2]->parameters[1]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[1].get());

	const auto &actions = normalizedDescription.domain->actions;

	REQUIRE(actions.size() == 2);

	CHECK(actions[0]->name == "test-action-1");
	CHECK(actions[0]->parameters.size() == 1);
	CHECK(actions[0]->parameters[0]->name == "x");
	CHECK(actions[0]->parameters[0]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[0].get());
	CHECK(actions[0]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[1].get());
	CHECK(actions[0]->effect.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[1].get());
	CHECK(actions[1]->name == "test-action-2");
	CHECK(actions[1]->parameters.size() == 2);
	CHECK(actions[1]->parameters[0]->name == "x");
	CHECK(actions[1]->parameters[0]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[0].get());
	CHECK(actions[1]->parameters[1]->name == "y");
	CHECK(actions[1]->parameters[1]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[1].get());
	const auto &pre1And = actions[1]->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
	REQUIRE(pre1And->arguments.size() == 2);
	const auto &pre1And1Not = pre1And->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>();
	CHECK(pre1And1Not->argument.get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[0].get());
	CHECK(pre1And->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[2].get());

	const auto &objects = normalizedDescription.problem.value()->objects;

	REQUIRE(objects.size() == 6);

	CHECK(objects[0]->name == "a");
	CHECK(objects[0]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[0].get());
	CHECK(objects[5]->name == "f");
	CHECK(objects[5]->type.value().get<pddl::normalizedAST::PrimitiveTypePointer>()->declaration == types[1].get());

	const auto &initialState = normalizedDescription.problem.value()->initialState;

	REQUIRE(initialState.facts.size() == 5);

	CHECK(initialState.facts[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[0].get());
	const auto &fact4 = initialState.facts[4].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
	CHECK(fact4->declaration == predicates[2].get());
	REQUIRE(fact4->arguments.size() == 2);
	CHECK(fact4->arguments[0].get<pddl::normalizedAST::ConstantPointer>()->declaration == objects[2].get());
	CHECK(fact4->arguments[1].get<pddl::normalizedAST::ConstantPointer>()->declaration == objects[5].get());

	const auto &goal = normalizedDescription.problem.value()->goal.value();
	const auto &goalAnd = goal.get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
	CHECK(goalAnd->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[0].get());
	const auto &goalAnd1Not = goalAnd->arguments[1].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>();
	CHECK(goalAnd1Not->argument.get<pddl::normalizedAST::PredicatePointer>()->declaration == predicates[1].get());
	const auto &goalAnd2 = goalAnd->arguments[2].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
	CHECK(goalAnd2->declaration == predicates[2].get());
	REQUIRE(goalAnd2->arguments.size() == 2);
	CHECK(goalAnd2->arguments[0].get<pddl::normalizedAST::ConstantPointer>()->declaration == objects[1].get());
	CHECK(goalAnd2->arguments[1].get<pddl::normalizedAST::ConstantPointer>()->declaration == objects[5].get());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] Implications are correctly reduced", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto domainFile = fs::path("data") / "normalization" / "normalization-2.pddl";
	context.tokenizer.read(domainFile);
	auto description = pddl::parseDescription(context);
	const auto normalizedDescription = pddl::normalize(std::move(description));

	const auto &actions = normalizedDescription.domain->actions;

	SECTION("top-level “imply” statement")
	{
		const auto &a1Pre = actions[0]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(a1Pre->arguments.size() == 1);
		CHECK(a1Pre->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration->name == "x");
		CHECK(a1Pre->declaration->existentialParameters.empty());
		CHECK(a1Pre->declaration->parameters.size() == 1);
		const auto &a1PreOr = a1Pre->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a1PreOr->arguments.size() == 2);
		const auto &a1PreOr1Not = a1PreOr->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a1PreOr1Not->declaration->name == "test-predicate-0");
		const auto &a1PreOr2 = a1PreOr->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a1PreOr2->declaration->name == "test-predicate-1");
	}

	SECTION("nested “imply” statement")
	{
		const auto &and1 = actions[1]->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and1->arguments.size() == 2);
		const auto &d1 = and1->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &and2 = d1->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and2->arguments.size() == 2);
		const auto &d2 = and2->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &or_ = d2->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(or_->arguments.size() == 2);
		const auto &or1Not = or_->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(or1Not->declaration->name == "test-predicate-0");
		const auto &or2 = or_->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(or2->declaration->name == "test-predicate-1");
	}

	SECTION("“imply” statement in goal description")
	{
		const auto &goal = normalizedDescription.problem.value()->goal.value();

		const auto &and_ = goal.get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and_->arguments.size() == 2);
		const auto &d = and_->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &or_ = d->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(or_->arguments.size() == 2);
		const auto &or1Not = or_->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(or1Not->declaration->name == "test-predicate-0");
		const auto &or2 = or_->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(or2->declaration->name == "test-predicate-1");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] Universal quantifiers are correctly reduced", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto domainFile = fs::path("data") / "normalization" / "normalization-3.pddl";
	context.tokenizer.read(domainFile);
	auto description = pddl::parseDescription(context);
	const auto normalizedDescription = pddl::normalize(std::move(description));

	const auto &actions = normalizedDescription.domain->actions;

	SECTION("top-level “forall” statement is replaced with negated “exists” statement over negated argument")
	{
		const auto &d = actions[0]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(d->arguments.empty());
		REQUIRE(d->declaration->existentialParameters.size() == 2);
		CHECK(d->declaration->existentialParameters[0]->name == "x");
		CHECK(d->declaration->existentialParameters[1]->name == "y");
		const auto &dp = d->declaration->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(dp->declaration->name == "test-predicate-2");
		REQUIRE(dp->declaration->parameters.size() == 2);
		CHECK(dp->declaration->parameters[0]->name == "x");
		CHECK(dp->declaration->parameters[1]->name == "y");
	}

	SECTION("nested “forall” statement is replaced with negated “exists” statement over negated argument")
	{
		const auto &and1 = actions[1]->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and1->arguments.size() == 2);
		const auto &d1 = and1->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &and2 = d1->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and2->arguments.size() == 2);
		const auto &d2 = and2->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(d2->arguments.empty());
		REQUIRE(d2->declaration->existentialParameters.size() == 2);
		CHECK(d2->declaration->existentialParameters[0]->name == "x");
		CHECK(d2->declaration->existentialParameters[1]->name == "y");
		const auto &d2p = d2->declaration->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(d2p->declaration->name == "test-predicate-2");
		REQUIRE(d2p->declaration->parameters.size() == 2);
		CHECK(d2p->declaration->parameters[0]->name == "x");
		CHECK(d2p->declaration->parameters[1]->name == "y");
	}

	SECTION("“forall” statement in goal description is replaced with negated “exists” statement over negated argument")
	{
		const auto &goal = normalizedDescription.problem.value()->goal.value();

		const auto &and_ = goal.get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(and_->arguments.size() == 2);
		const auto &d = and_->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(d->arguments.empty());
		REQUIRE(d->declaration->existentialParameters.size() == 2);
		CHECK(d->declaration->existentialParameters[0]->name == "x");
		CHECK(d->declaration->existentialParameters[1]->name == "y");
		const auto &dp = d->declaration->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(dp->declaration->name == "test-predicate-2");
		REQUIRE(dp->declaration->parameters.size() == 2);
		CHECK(dp->declaration->parameters[0]->name == "x");
		CHECK(dp->declaration->parameters[1]->name == "y");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] Negations are correctly normalized", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto domainFile = fs::path("data") / "normalization" / "normalization-4.pddl";
	context.tokenizer.read(domainFile);
	auto description = pddl::parseDescription(context);
	const auto normalizedDescription = pddl::normalize(std::move(description));

	const auto &actions = normalizedDescription.domain->actions;

	SECTION("multiple negations are correctly eliminated")
	{
		const auto &p1 = actions[0]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(p1->declaration->name == "test-predicate-1");
		REQUIRE(p1->arguments.size() == 1);
		CHECK(p1->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration->name == "x");

		const auto &p2 = actions[1]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(p2->declaration->name == "test-predicate-1");
		REQUIRE(p2->arguments.size() == 1);
		CHECK(p2->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration->name == "x");
	}

	SECTION("“exists” statements with negations are correctly normalized")
	{
		const auto &dp = actions[2]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		CHECK(dp->declaration->existentialParameters.size() == 2);
		const auto &dpp = dp->declaration->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(dpp->declaration->name == "test-predicate-2");
	}

	SECTION("“forall” statements with negations are correctly normalized")
	{
		const auto &dp = actions[3]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		CHECK(dp->declaration->existentialParameters.size() == 2);
		const auto &dpp = dp->declaration->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(dpp->declaration->name == "test-predicate-2");
		}

	SECTION("negations introduced by reduction are correctly normalized")
	{
		const auto &d = actions[4]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &do_ = d->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(do_->arguments.size() == 2);
		const auto &do1 = do_->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(do1->declaration->name == "test-predicate-0");
		const auto &do2 = do_->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(do2->declaration->name == "test-predicate-1");
	}

	SECTION("negated disjunctions are correctly normalized")
	{
		const auto &a = actions[5]->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a->arguments.size() == 2);
		const auto &a1 = a->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(a1->declaration->name == "test-predicate-0");
		const auto &a2 = a->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(a2->declaration->name == "test-predicate-1");
	}

	SECTION("negated conjunctions are correctly normalized")
	{
		const auto &d = actions[6]->precondition.value().get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &do_ = d->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(do_->arguments.size() == 2);
		const auto &do1 = do_->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(do1->declaration->name == "test-predicate-0");
		const auto &do2 = do_->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		REQUIRE(do2->declaration->name == "test-predicate-1");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] Nested expressions are correctly flattened via derived predicates", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto domainFile = fs::path("data") / "normalization" / "normalization-5.pddl";
	context.tokenizer.read(domainFile);
	auto description = pddl::parseDescription(context);
	const auto normalizedDescription = pddl::normalize(std::move(description));

	const auto &actions = normalizedDescription.domain->actions;

	SECTION("nested expressions in action preconditions are correctly flattened")
	{
		const auto &a = actions[0]->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a->arguments.size() == 3);

		const auto &a1 = a->arguments[0].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(a1->declaration->parameters.size() == 1);
		CHECK(a1->declaration->existentialParameters.size() == 1);
		const auto &a11 = a1->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a11->arguments.size() == 2);
		const auto &a111 = a11->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a111->declaration->name == "p1");
		REQUIRE(a111->arguments.size() == 2);
		CHECK(a111->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());
		CHECK(a111->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == a1->declaration->existentialParameters[0].get());
		const auto &a112 = a11->arguments[1].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a112->declaration->name == "p2");

		const auto &a2 = a->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a2->declaration->name == "p3");
		REQUIRE(a2->arguments.size() == 2);
		CHECK(a2->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[0].get());

		const auto &a3 = a->arguments[2].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(a3->arguments.size() == 2);
		const auto &a31 = a3->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a31->arguments.size() == 2);
		const auto &a311 = a31->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &a3111 = a311->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a3111->arguments.size() == 2);
		const auto &a31111 = a3111->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31111->declaration->name == "p4");
		const auto &a31112 = a3111->arguments[1].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		CHECK(a31112->declaration->parameters.size() == 2);
		CHECK(a31112->declaration->existentialParameters.size() == 1);
		const auto &a311121 = a31112->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a311121->arguments.size() == 2);
		const auto &a3111211 = a311121->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a3111211->declaration->name == "p5");
		const auto &a3111212 = a311121->arguments[1].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a3111212->declaration->name == "p3");
		const auto &a312 = a31->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		const auto &a3121 = a312->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a3121->arguments.size() == 2);
		const auto &a31211 = a3121->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31111->declaration->name == "p4");
		const auto &a31212 = a3121->arguments[1].get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::DerivedPredicatePointer>();
		REQUIRE(a31212->declaration->parameters.size() == 1);
		REQUIRE(a31212->declaration->existentialParameters.size() == 2);
		const auto &a312121 = a31212->declaration->precondition.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a312121->arguments.size() == 2);
		const auto &a3121211 = a312121->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a3121211->declaration->name == "p6");
		REQUIRE(a3121211->arguments.size() == 3);
		CHECK(a3121211->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == a31212->declaration->existentialParameters[0].get());
		CHECK(a3121211->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == a31212->declaration->existentialParameters[1].get());
		CHECK(a3121211->arguments[2].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());
		const auto &a3121212 = a312121->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a3121212->declaration->name == "p3");
		REQUIRE(a3121212->arguments.size() == 2);
		CHECK(a3121212->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == a31212->declaration->existentialParameters[0].get());
		CHECK(a3121212->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());
	}

	SECTION("nested expressions in conditional effect preconditions are correctly flattened")
	{
		const auto &a = actions[0]->effect.value().get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Effect>>();
		REQUIRE(a->arguments.size() == 3);

		const auto &a1 = a->arguments[0].get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::NotPointer<pddl::normalizedAST::AtomicFormula>>()->argument.get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a1->declaration->name == "p3");
		REQUIRE(a1->arguments.size() == 2);
		CHECK(a1->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[0].get());
		CHECK(a1->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());

		const auto &a2 = a->arguments[1].get<pddl::normalizedAST::Literal>().get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a2->declaration->name == "p9");
		REQUIRE(a2->arguments.size() == 1);
		CHECK(a2->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[0].get());

		const auto &a3 = a->arguments[2].get<pddl::normalizedAST::WhenPointer<pddl::normalizedAST::Precondition, pddl::normalizedAST::ConditionalEffect>>();
		const auto &a31 = a3->argumentLeft.get<pddl::normalizedAST::AndPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a31->arguments.size() == 2);
		const auto &a311 = a31->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		CHECK(a311->declaration->parameters.size() == 1);
		CHECK(a311->declaration->existentialParameters.size() == 1);
		const auto &a3111 = a311->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		REQUIRE(a3111->arguments.size() == 2);
		const auto &a31111 = a3111->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31111->declaration->name == "p7");
		REQUIRE(a31111->arguments.size() == 2);
		CHECK(a31111->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == a311->declaration->existentialParameters[0].get());
		CHECK(a31111->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());
		const auto &a31112 = a3111->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31112->declaration->name == "p3");
		REQUIRE(a31112->arguments.size() == 2);
		CHECK(a31112->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == actions[0]->parameters[1].get());
		CHECK(a31112->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == a311->declaration->existentialParameters[0].get());
		const auto &a312 = a31->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::DerivedPredicatePointer>();
		CHECK(a312->declaration->parameters.size() == 0);
		CHECK(a312->declaration->existentialParameters.size() == 2);
		const auto &a3121 = a312->declaration->precondition.value().get<pddl::normalizedAST::OrPointer<pddl::normalizedAST::Literal>>();
		const auto &a31211 = a3121->arguments[0].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31211->declaration->name == "p4");
		REQUIRE(a31211->arguments.size() == 2);
		CHECK(a31211->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == a312->declaration->existentialParameters[0].get());
		CHECK(a31211->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == a312->declaration->existentialParameters[1].get());
		const auto &a31212 = a3121->arguments[1].get<pddl::normalizedAST::AtomicFormula>().get<pddl::normalizedAST::PredicatePointer>();
		CHECK(a31212->declaration->name == "p3");
		REQUIRE(a31212->arguments.size() == 2);
		CHECK(a31212->arguments[0].get<pddl::normalizedAST::VariablePointer>()->declaration == a312->declaration->existentialParameters[1].get());
		CHECK(a31212->arguments[1].get<pddl::normalizedAST::VariablePointer>()->declaration == a312->declaration->existentialParameters[0].get());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[normalization] Derived predicates are correctly numbered", "[normalization]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("derived predicates introduced by both domain and problem")
	{
		const auto domainFile = fs::path("data") / "normalization" / "normalization-6-1.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);
		const auto normalizedDescription = pddl::normalize(std::move(description));

		const auto &domain = normalizedDescription.domain;
		const auto &problem = normalizedDescription.problem.value();

		REQUIRE(domain->derivedPredicates.size() == 4);
		REQUIRE(problem->derivedPredicates.size() == 2);

		CHECK(domain->derivedPredicates[0]->name == "derived-predicate-1");
		CHECK(domain->derivedPredicates[1]->name == "derived-predicate-2");
		CHECK(domain->derivedPredicates[2]->name == "derived-predicate-3");
		CHECK(domain->derivedPredicates[3]->name == "derived-predicate-4");
		CHECK(problem->derivedPredicates[0]->name == "derived-predicate-5");
		CHECK(problem->derivedPredicates[1]->name == "derived-predicate-6");
	}

	SECTION("derived predicates introduced by domain only")
	{
		const auto domainFile = fs::path("data") / "normalization" / "normalization-6-2.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);
		const auto normalizedDescription = pddl::normalize(std::move(description));

		const auto &domain = normalizedDescription.domain;
		REQUIRE(!normalizedDescription.problem);

		REQUIRE(domain->derivedPredicates.size() == 4);

		CHECK(domain->derivedPredicates[0]->name == "derived-predicate-1");
		CHECK(domain->derivedPredicates[1]->name == "derived-predicate-2");
		CHECK(domain->derivedPredicates[2]->name == "derived-predicate-3");
		CHECK(domain->derivedPredicates[3]->name == "derived-predicate-4");
	}

	SECTION("derived predicates introduced by both domain and problem")
	{
		const auto domainFile = fs::path("data") / "normalization" / "normalization-6-3.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);
		const auto normalizedDescription = pddl::normalize(std::move(description));

		const auto &domain = normalizedDescription.domain;
		const auto &problem = normalizedDescription.problem.value();

		REQUIRE(domain->derivedPredicates.empty());
		REQUIRE(problem->derivedPredicates.size() == 2);

		CHECK(problem->derivedPredicates[0]->name == "derived-predicate-1");
		CHECK(problem->derivedPredicates[1]->name == "derived-predicate-2");
	}
}
