#include <catch.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <plasp/pddl/Description.h>
#include <plasp/pddl/expressions/And.h>
#include <plasp/pddl/expressions/Either.h>
#include <plasp/pddl/expressions/Not.h>
#include <plasp/pddl/expressions/Predicate.h>
#include <plasp/pddl/expressions/PrimitiveType.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] The Blocks World domain is parsed correctly", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	const auto description = Description::fromFile("data/blocksworld-domain.pddl", context);

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
	REQUIRE(on.arguments().size() == 2u);
	CHECK(on.arguments()[0]->name() == "x");
	const auto &onArgument0Type = dynamic_cast<const expressions::PrimitiveType &>(*on.arguments()[0]->type());
	CHECK(&onArgument0Type == &block);
	CHECK(on.arguments()[1]->name() == "y");
	const auto &onArgument1Type = dynamic_cast<const expressions::PrimitiveType &>(*on.arguments()[1]->type());
	CHECK(&onArgument1Type == &block);

	const auto &handempty = *domain.predicates()[3];

	CHECK(handempty.name() == "handempty");
	CHECK(handempty.arguments().empty());

	// Actions
	REQUIRE(domain.actions().size() == 4u);

	const auto &pickUp = *domain.actions()[0];

	CHECK(pickUp.name() == "pick-up");
	REQUIRE(pickUp.parameters().size() == 1u);
	CHECK(pickUp.parameters()[0]->name() == "x");
	CHECK(pickUp.parameters()[0]->type() == &block);

	const auto &pickUpPre = dynamic_cast<const expressions::And &>(*pickUp.precondition());
	REQUIRE(pickUpPre.arguments().size() == 3u);
	const auto &pickUpPre0 = dynamic_cast<const expressions::Predicate &>(*pickUpPre.arguments()[0]);
	CHECK(pickUpPre0.name() == "clear");
	REQUIRE(pickUpPre0.arguments().size() == 1u);
	const auto &pickUpPre00 = dynamic_cast<const expressions::Variable &>(*pickUpPre0.arguments()[0]);
	CHECK(pickUpPre00.name() == "x");
	CHECK(pickUpPre00.type() == &block);
	CHECK(&pickUpPre00 == pickUp.parameters()[0].get());
	const auto &pickUpPre2 = dynamic_cast<const expressions::Predicate &>(*pickUpPre.arguments()[2]);
	CHECK(pickUpPre2.name() == "handempty");
	CHECK(pickUpPre2.arguments().empty());

	const auto &pickUpEff = dynamic_cast<const expressions::And &>(*pickUp.effect());
	REQUIRE(pickUpEff.arguments().size() == 4u);
	const auto &pickUpEff0 = dynamic_cast<const expressions::Not &>(*pickUpEff.arguments()[0]);
	const auto &pickUpEff00 = dynamic_cast<const expressions::Predicate &>(*pickUpEff0.argument());
	CHECK(pickUpEff00.name() == "ontable");
	REQUIRE(pickUpEff00.arguments().size() == 1u);
	const auto &pickUpEff000 = dynamic_cast<const expressions::Variable &>(*pickUpEff00.arguments()[0]);
	CHECK(pickUpEff000.name() == "x");
	CHECK(pickUpEff000.type() == &block);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] A Blocks World problem is parsed correctly", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	const auto description = Description::fromFiles({"data/blocksworld-domain.pddl", "data/blocksworld-problem.pddl"}, context);

	REQUIRE_NOTHROW(description.problem());

	const auto &problem = description.problem();

	// Name
	CHECK(problem.name() == "blocks-4-0");
	CHECK(problem.domain().name() == "blocks");

	// Requirements
	// TODO: compute domain vs. problem requirements correctly and check them

	// Objects
	REQUIRE(problem.objects().size() == 4u);

	CHECK(problem.objects()[0]->name() == "d");
	REQUIRE(problem.objects()[0]->type() != nullptr);
	CHECK(problem.objects()[0]->type()->name() == "block");
	CHECK(problem.objects()[3]->name() == "c");
	REQUIRE(problem.objects()[3]->type() != nullptr);
	CHECK(problem.objects()[3]->type()->name() == "block");

	// Initial State
	const auto &facts = problem.initialState().facts();

	REQUIRE(facts.size() == 9u);
	const auto &fact0 = dynamic_cast<const expressions::Predicate &>(*facts[0].get());
	CHECK(fact0.name() == "clear");
	REQUIRE(fact0.arguments().size() == 1u);
	const auto &fact00 = dynamic_cast<const expressions::Constant &>(*fact0.arguments()[0]);
	CHECK(fact00.name() == "c");
	REQUIRE(fact00.type() != nullptr);
	CHECK(fact00.type()->name() == "block");
	const auto &fact8 = dynamic_cast<const expressions::Predicate &>(*facts[8].get());
	CHECK(fact8.name() == "handempty");
	REQUIRE(fact8.arguments().size() == 0u);

	// Goal
	const auto &goal = dynamic_cast<const expressions::And &>(problem.goal());

	REQUIRE(goal.arguments().size() == 3u);
	const auto &goal0 = dynamic_cast<const expressions::Predicate &>(*goal.arguments()[0]);
	CHECK(goal0.name() == "on");
	REQUIRE(goal0.arguments().size() == 2u);
	const auto &goal00 = dynamic_cast<const expressions::Constant &>(*goal0.arguments()[0]);
	CHECK(goal00.name() == "d");
	const auto &goal01 = dynamic_cast<const expressions::Constant &>(*goal0.arguments()[1]);
	CHECK(goal01.name() == "c");
	const auto &goal2 = dynamic_cast<const expressions::Predicate &>(*goal.arguments()[2]);
	CHECK(goal2.name() == "on");
	REQUIRE(goal2.arguments().size() == 2u);
	const auto &goal20 = dynamic_cast<const expressions::Constant &>(*goal2.arguments()[0]);
	CHECK(goal20.name() == "b");
	const auto &goal21 = dynamic_cast<const expressions::Constant &>(*goal2.arguments()[1]);
	CHECK(goal21.name() == "a");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] The Storage domain is parsed correctly", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	const auto description = plasp::pddl::Description::fromFile("data/storage-domain.pddl", context);

	REQUIRE_NOTHROW(description.domain());

	const auto &domain = description.domain();

	// Name
	CHECK(domain.name() == "storage-propositional");

	// Requirements
	REQUIRE(domain.requirements().size() == 1u);
	CHECK(domain.requirements()[0].type() == Requirement::Type::Typing);

	// Types
	REQUIRE(domain.types().size() == 10u);

	const auto &hoist = *domain.types()[0];
	const auto &surface = *domain.types()[1];
	const auto &area = *domain.types()[3];
	const auto &object = *domain.types()[4];
	const auto &storearea = *domain.types()[7];
	const auto &crate = *domain.types()[9];

	const auto &hoistParents = hoist.parentTypes();
	REQUIRE(hoistParents.size() == 1u);
	CHECK(std::find(hoistParents.cbegin(), hoistParents.cend(), &object) != hoistParents.cend());

	const auto &areaParents = area.parentTypes();
	REQUIRE(areaParents.size() == 2u);
	CHECK(std::find(areaParents.cbegin(), areaParents.cend(), &object) != areaParents.cend());
	CHECK(std::find(areaParents.cbegin(), areaParents.cend(), &surface) != areaParents.cend());

	// Predicates
	REQUIRE(domain.predicates().size() == 8u);

	const auto &on = *domain.predicates()[5];

	CHECK(on.name() == "on");
	REQUIRE(on.arguments().size() == 2u);
	CHECK(on.arguments()[0]->name() == "c");
	const auto &onArgument0Type = dynamic_cast<const expressions::PrimitiveType &>(*on.arguments()[0]->type());
	CHECK(&onArgument0Type == &crate);
	CHECK(on.arguments()[1]->name() == "s");
	const auto &onArgument1Type = dynamic_cast<const expressions::PrimitiveType &>(*on.arguments()[1]->type());
	CHECK(&onArgument1Type == &storearea);

	const auto &in = *domain.predicates()[1];
	CHECK(in.name() == "in");
	REQUIRE(in.arguments().size() == 2u);
	CHECK(in.arguments()[0]->name() == "x");
	const auto &inArgument0Type = dynamic_cast<const expressions::Either &>(*in.arguments()[0]->type());
	REQUIRE(inArgument0Type.arguments().size() == 2u);
	const auto &inArgument0Type0 = dynamic_cast<const expressions::PrimitiveType &>(*inArgument0Type.arguments()[0]);
	CHECK(&inArgument0Type0 == &storearea);
	const auto &inArgument0Type1 = dynamic_cast<const expressions::PrimitiveType &>(*inArgument0Type.arguments()[1]);
	CHECK(&inArgument0Type1 == &crate);

	// Actions
	REQUIRE(domain.actions().size() == 5u);

	const auto &drop = *domain.actions()[1];

	CHECK(drop.name() == "drop");
	REQUIRE(drop.parameters().size() == 5u);
	CHECK(drop.parameters()[3]->name() == "a2");
	CHECK(drop.parameters()[3]->type() == &area);

	const auto &dropPre = dynamic_cast<const expressions::And &>(*drop.precondition());
	REQUIRE(dropPre.arguments().size() == 5u);
	const auto &dropPre2 = dynamic_cast<const expressions::Predicate &>(*dropPre.arguments()[2]);
	CHECK(dropPre2.name() == "lifting");
	REQUIRE(dropPre2.arguments().size() == 2u);
	const auto &dropPre21 = dynamic_cast<const expressions::Variable &>(*dropPre2.arguments()[1]);
	CHECK(dropPre21.name() == "c");
	CHECK(dropPre21.type() == &crate);

	const auto &dropEff = dynamic_cast<const expressions::And &>(*drop.effect());
	REQUIRE(dropEff.arguments().size() == 5u);
	const auto &dropEff2 = dynamic_cast<const expressions::Not &>(*dropEff.arguments()[2]);
	const auto &dropEff20 = dynamic_cast<const expressions::Predicate &>(*dropEff2.argument());
	CHECK(dropEff20.name() == "clear");
	REQUIRE(dropEff20.arguments().size() == 1u);
	const auto &dropEff200 = dynamic_cast<const expressions::Variable &>(*dropEff20.arguments()[0]);
	CHECK(dropEff200.name() == "a1");
	CHECK(dropEff200.type() == &storearea);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] A Storage problem is parsed correctly", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	const auto description = Description::fromFiles({"data/storage-domain.pddl", "data/storage-problem.pddl"}, context);

	REQUIRE_NOTHROW(description.problem());

	const auto &problem = description.problem();

	// Name
	CHECK(problem.name() == "storage-1");
	CHECK(problem.domain().name() == "storage-propositional");

	// Requirements
	// TODO: compute domain vs. problem requirements correctly and check them

	// Objects
	REQUIRE(problem.objects().size() == 7u);

	CHECK(problem.objects()[0]->name() == "depot0-1-1");
	REQUIRE(problem.objects()[0]->type() != nullptr);
	CHECK(problem.objects()[0]->type()->name() == "storearea");
	CHECK(problem.objects()[6]->name() == "loadarea");
	REQUIRE(problem.objects()[6]->type() != nullptr);
	CHECK(problem.objects()[6]->type()->name() == "transitarea");

	// Initial State
	const auto &facts = problem.initialState().facts();

	REQUIRE(facts.size() == 10u);
	const auto &fact0 = dynamic_cast<const expressions::Predicate &>(*facts[0].get());
	CHECK(fact0.name() == "in");
	REQUIRE(fact0.arguments().size() == 2u);
	const auto &fact01 = dynamic_cast<const expressions::Constant &>(*fact0.arguments()[1]);
	CHECK(fact01.name() == "depot0");
	REQUIRE(fact01.type() != nullptr);
	CHECK(fact01.type()->name() == "depot");
	const auto &fact9 = dynamic_cast<const expressions::Predicate &>(*facts[9].get());
	CHECK(fact9.name() == "available");
	REQUIRE(fact9.arguments().size() == 1u);
	const auto &fact90 = dynamic_cast<const expressions::Constant &>(*fact9.arguments()[0]);
	CHECK(fact90.name() == "hoist0");
	REQUIRE(fact90.type() != nullptr);
	CHECK(fact90.type()->name() == "hoist");

	// Goal
	const auto &goal = dynamic_cast<const expressions::And &>(problem.goal());

	REQUIRE(goal.arguments().size() == 1u);
	const auto &goal0 = dynamic_cast<const expressions::Predicate &>(*goal.arguments()[0]);
	CHECK(goal0.name() == "in");
	REQUIRE(goal0.arguments().size() == 2u);
	const auto &goal00 = dynamic_cast<const expressions::Constant &>(*goal0.arguments()[0]);
	CHECK(goal00.name() == "crate0");
	const auto &goal01 = dynamic_cast<const expressions::Constant &>(*goal0.arguments()[1]);
	CHECK(goal01.name() == "depot0");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] Constants are parsed correctly", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	const auto description = Description::fromFile("data/woodworking-domain.pddl", context);

	REQUIRE_NOTHROW(description.domain());

	const auto &domain = description.domain();

	// Name
	CHECK(domain.name() == "woodworking");

	// Types
	const auto &acolour = *domain.types()[0];
	const auto &surface = *domain.types()[4];
	const auto &treatmentstatus = *domain.types()[5];

	// Constants
	REQUIRE(domain.constants().size() == 8u);
	CHECK(domain.constants()[0]->type() == &surface);
	CHECK(domain.constants()[2]->type() == &surface);
	CHECK(domain.constants()[3]->type() == &treatmentstatus);
	CHECK(domain.constants()[6]->type() == &treatmentstatus);
	CHECK(domain.constants()[7]->type() == &acolour);

	// TODO: add test with constants in predicates
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] White spaces are ignored", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	CHECK_NOTHROW(Description::fromFile("data/white-space-test.pddl", context));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] Missing or unmatching domain descriptions are detected", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	SECTION("")
	{
		CHECK_THROWS_AS(Description::fromFile("data/blocksworld-problem.pddl", context), ConsistencyException);
	}
	SECTION("")
	{
		CHECK_THROWS_AS(Description::fromFiles({"data/blocksworld-problem.pddl", "data/storage-domain.pddl"}, context), plasp::input::ParserException);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] Common PDDL syntax errors are detected", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	SECTION("")
	{
		CHECK_NOTHROW(Description::fromFile("data/pddl-syntax/domain-valid.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expressions-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expressions-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expressions-3.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expression-name-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expression-name-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-expression-name-3.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-3.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-4.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-5.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-6.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-7.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-parentheses-8.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-3.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-4.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-5.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-6.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-section-name-7.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-types-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-types-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-types-3.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-types-4.pddl", context));
	}

	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-variables-1.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-variables-2.pddl", context));
	}
	SECTION("")
	{
		CHECK_THROWS(Description::fromFile("data/pddl-syntax/domain-variables-3.pddl", context));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] Former issues are fixed", "[PDDL parser]")
{
	plasp::output::Logger logger;
	Context context(Parser(), logger);

	SECTION("white space issues with constants and parsing unsupported sections")
	{
		CHECK_NOTHROW(Description::fromFile("data/issues/issue-1.pddl", context));
	}

	SECTION("white space issues with empty n-ary predicates")
	{
		CHECK_NOTHROW(Description::fromFile("data/issues/issue-2.pddl", context));
	}

	SECTION("comments are correctly ignored")
	{
		CHECK_NOTHROW(Description::fromFile("data/issues/issue-3.pddl", context));
	}
}
