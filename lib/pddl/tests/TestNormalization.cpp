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
