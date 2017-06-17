#include <catch.hpp>

#include <experimental/filesystem>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] The official PDDL instances are parsed correctly", "[PDDL parser]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("types, predicates, and actions in blocksworld domain")
	{
		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "blocks-strips-typed" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &types = description.domain->types;

		REQUIRE(types.size() == 1);
		const auto &typeBlock = types[0];
		CHECK(typeBlock->name == "block");

		const auto &predicates = description.domain->predicates;

		REQUIRE(predicates.size() == 5);

		CHECK(predicates[0]->name == "on");
		REQUIRE(predicates[0]->parameters.size() == 2);
		CHECK(predicates[0]->parameters[0]->name == "x");
		CHECK(predicates[0]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(predicates[0]->parameters[1]->name == "y");
		CHECK(predicates[0]->parameters[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(predicates[1]->name == "ontable");
		REQUIRE(predicates[1]->parameters.size() == 1);
		CHECK(predicates[1]->parameters[0]->name == "x");
		CHECK(predicates[1]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(predicates[2]->name == "clear");
		REQUIRE(predicates[2]->parameters.size() == 1);
		CHECK(predicates[2]->parameters[0]->name == "x");
		CHECK(predicates[2]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(predicates[3]->name == "handempty");
		CHECK(predicates[3]->parameters.empty());
		CHECK(predicates[4]->name == "holding");
		REQUIRE(predicates[4]->parameters.size() == 1);
		CHECK(predicates[4]->parameters[0]->name == "x");
		CHECK(predicates[4]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());

		const auto &actions = description.domain->actions;

		REQUIRE(actions.size() == 4);

		CHECK(actions[3]->name == "unstack");

		REQUIRE(actions[3]->parameters.size() == 2);
		CHECK(actions[3]->parameters[0]->name == "x");
		CHECK(actions[3]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(actions[3]->parameters[1]->name == "y");
		CHECK(actions[3]->parameters[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());

		const auto &preconditionAnd = actions[3]->precondition.value().get<pddl::ast::AndPointer<pddl::ast::Precondition>>();
		const auto &precondition0 = preconditionAnd->arguments[0].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(precondition0->arguments.size() == 2);
		CHECK(precondition0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(precondition0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(precondition0->arguments[1].get<pddl::ast::VariablePointer>()->declaration->name == "y");
		CHECK(precondition0->arguments[1].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &precondition1 = preconditionAnd->arguments[1].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(precondition1->arguments.size() == 1);
		CHECK(precondition1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(precondition1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &precondition2 = preconditionAnd->arguments[2].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(precondition2->arguments.empty());

		const auto &effectAnd = actions[3]->effect.value().get<pddl::ast::AndPointer<pddl::ast::Effect>>();
		const auto &effect0 = effectAnd->arguments[0].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effect0->arguments.size() == 1);
		CHECK(effect0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effect0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effect1 = effectAnd->arguments[1].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effect1->arguments.size() == 1);
		CHECK(effect1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "y");
		CHECK(effect1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effectNot2 = effectAnd->arguments[2].get<pddl::ast::NotPointer<pddl::ast::Effect>>()->argument.get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot2->arguments.size() == 1);
		CHECK(effectNot2->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effectNot2->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effectNot3 = effectAnd->arguments[3].get<pddl::ast::NotPointer<pddl::ast::Effect>>()->argument.get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot3->arguments.empty());
		const auto &effectNot4 = effectAnd->arguments[4].get<pddl::ast::NotPointer<pddl::ast::Effect>>()->argument.get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot4->arguments.size() == 2);
		CHECK(effectNot4->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effectNot4->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(effectNot4->arguments[1].get<pddl::ast::VariablePointer>()->declaration->name == "y");
		CHECK(effectNot4->arguments[1].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
	}

	SECTION("“either” type in zenotravel domain")
	{
		const auto domainFile = pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-numeric-hand-coded" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &predicates = description.domain->predicates;

		REQUIRE(predicates.size() == 2);
		REQUIRE(predicates[0]->name == "at");
		REQUIRE(predicates[0]->parameters.size() == 2);
		REQUIRE(predicates[0]->parameters[0]->name == "x");
		REQUIRE(predicates[0]->parameters[0]->type);
		REQUIRE(predicates[0]->parameters[0]->type.value().is<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>());
		CHECK(predicates[0]->parameters[0]->type.value().get<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>()->arguments[0]->declaration->name == "person");
		CHECK(predicates[0]->parameters[0]->type.value().get<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>()->arguments[1]->declaration->name == "aircraft");
		REQUIRE(predicates[0]->parameters[1]->name == "c");
		REQUIRE(predicates[0]->parameters[1]->type);
		REQUIRE(predicates[0]->parameters[1]->type.value().is<pddl::ast::PrimitiveTypePointer>());
		CHECK(predicates[0]->parameters[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "city");
	}
}
