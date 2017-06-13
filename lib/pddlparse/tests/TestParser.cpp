/*#include <catch.hpp>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser] The Blocksworld domain is parsed correctly", "[parser]")
{
	const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

	pddl::Tokenizer tokenizer;
	tokenizer.read("data/blocksworld-domain.pddl");
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	const auto description = pddl::parseDescription(context);

	REQUIRE_NOTHROW(description.domain());

	const auto &domain = description.domain();

	// Name
	CHECK(domain.name() == "blocks");

	// Requirements
	REQUIRE(domain.requirements().size() == 2u);
	CHECK(domain.requirements()[0].type() == Requirement::Type::STRIPS);
	CHECK(domain.requirements()[1].type() == Requirement::Type::Typing);

	// Types
	REQUIRE(domain.types().size() == 1u);

	const auto &block = *domain.types()[0];

	CHECK(block.name() == "block");
	REQUIRE(block.parentTypes().size() == 0u);

	// Predicates
	REQUIRE(domain.predicates().size() == 5u);

	const auto &on = *domain.predicates()[0];

	CHECK(on.name() == "on");
	REQUIRE(on.parameters().size() == 2u);
	CHECK(on.parameters()[0]->name() == "x");
	const auto &onArgument0Type = on.parameters()[0]->type()->as<expressions::PrimitiveType>();
	CHECK(&onArgument0Type == &block);
	CHECK(on.parameters()[1]->name() == "y");
	const auto &onArgument1Type = on.parameters()[1]->type()->as<expressions::PrimitiveType>();
	CHECK(&onArgument1Type == &block);

	const auto &handempty = *domain.predicates()[3];

	CHECK(handempty.name() == "handempty");
	CHECK(handempty.parameters().empty());

	// Actions
	REQUIRE(domain.actions().size() == 4u);

	const auto &pickUp = *domain.actions()[0];

	CHECK(pickUp.name() == "pick-up");
	REQUIRE(pickUp.parameters().size() == 1u);
	CHECK(pickUp.parameters()[0]->name() == "x");
	CHECK(pickUp.parameters()[0]->type() == &block);

	const auto &pickUpPre = pickUp.precondition()->as<expressions::And>();
	REQUIRE(pickUpPre.arguments().size() == 3u);
	const auto &pickUpPre0 = pickUpPre.arguments()[0]->as<expressions::Predicate>();
	CHECK(pickUpPre0.name() == "clear");
	REQUIRE(pickUpPre0.arguments().size() == 1u);
	const auto &pickUpPre00 = pickUpPre0.arguments()[0]->as<expressions::Variable>();
	CHECK(pickUpPre00.name() == "x");
	CHECK(pickUpPre00.type() == &block);
	CHECK(&pickUpPre00 == pickUp.parameters()[0].get());
	const auto &pickUpPre2 = pickUpPre.arguments()[2]->as<expressions::Predicate>();
	CHECK(pickUpPre2.name() == "handempty");
	CHECK(pickUpPre2.arguments().empty());

	const auto &pickUpEff = pickUp.effect()->as<expressions::And>();
	REQUIRE(pickUpEff.arguments().size() == 4u);
	const auto &pickUpEff0 = pickUpEff.arguments()[0]->as<expressions::Not>();
	const auto &pickUpEff00 = pickUpEff0.argument()->as<expressions::Predicate>();
	CHECK(pickUpEff00.name() == "ontable");
	REQUIRE(pickUpEff00.arguments().size() == 1u);
	const auto &pickUpEff000 = pickUpEff00.arguments()[0]->as<expressions::Variable>();
	CHECK(pickUpEff000.name() == "x");
	CHECK(pickUpEff000.type() == &block);
}
*/
