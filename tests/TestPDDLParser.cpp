#include <gtest/gtest.h>

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
#include <plasp/pddl/expressions/Reference.h>

using namespace plasp::pddl;

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseBlocksWorldDomain)
{
	const auto description = Description::fromFile("data/blocksworld-domain.pddl");

	ASSERT_NO_THROW(description.domain());

	const auto &domain = description.domain();

	// Name
	ASSERT_EQ(domain.name(), "blocks");

	// Requirements
	ASSERT_EQ(domain.requirements().size(), 2u);
	ASSERT_EQ(domain.requirements()[0].type(), Requirement::Type::STRIPS);
	ASSERT_EQ(domain.requirements()[1].type(), Requirement::Type::Typing);

	// Types
	ASSERT_EQ(domain.types().size(), 1u);

	const auto &block = *domain.types()[0];

	ASSERT_EQ(block.name(), "block");
	ASSERT_EQ(block.parentTypes().size(), 0u);

	// Predicates
	ASSERT_EQ(domain.predicates().size(), 5u);

	const auto &on = *domain.predicates()[0];

	ASSERT_EQ(on.name(), "on");
	ASSERT_EQ(on.arguments().size(), 2u);
	ASSERT_EQ(on.arguments()[0]->name(), "x");
	const auto *onArgument0Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[0]->type());
	ASSERT_EQ(onArgument0Type, &block);
	ASSERT_EQ(on.arguments()[1]->name(), "y");
	const auto onArgument1Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[1]->type());
	ASSERT_EQ(onArgument1Type, &block);

	const auto &handempty = *domain.predicates()[3];

	ASSERT_EQ(handempty.name(), "handempty");
	ASSERT_TRUE(handempty.arguments().empty());

	// Actions
	ASSERT_EQ(domain.actions().size(), 4u);

	const auto &pickUp = *domain.actions()[0];

	ASSERT_EQ(pickUp.name(), "pick-up");
	ASSERT_EQ(pickUp.parameters().size(), 1u);
	ASSERT_EQ(pickUp.parameters()[0]->name(), "x");
	ASSERT_EQ(pickUp.parameters()[0]->type(), &block);

	const auto &pickUpPre = dynamic_cast<const expressions::And &>(pickUp.precondition());
	ASSERT_EQ(pickUpPre.arguments().size(), 3u);
	const auto &pickUpPre0 = dynamic_cast<const expressions::Predicate &>(*pickUpPre.arguments()[0]);
	ASSERT_EQ(pickUpPre0.name(), "clear");
	ASSERT_EQ(pickUpPre0.arguments().size(), 1u);
	const auto &pickUpPre00 = *dynamic_cast<const expressions::Reference<expressions::Variable> &>(*pickUpPre0.arguments()[0]).value();
	ASSERT_EQ(pickUpPre00.name(), "x");
	ASSERT_EQ(pickUpPre00.type(), &block);
	ASSERT_EQ(&pickUpPre00, pickUp.parameters()[0].get());
	const auto &pickUpPre2 = dynamic_cast<const expressions::Predicate &>(*pickUpPre.arguments()[2]);
	ASSERT_EQ(pickUpPre2.name(), "handempty");
	ASSERT_EQ(pickUpPre2.arguments().size(), 0u);

	const auto &pickUpEff = dynamic_cast<const expressions::And &>(pickUp.effect());
	ASSERT_EQ(pickUpEff.arguments().size(), 4u);
	const auto &pickUpEff0 = dynamic_cast<const expressions::Not &>(*pickUpEff.arguments()[0]);
	const auto &pickUpEff00 = dynamic_cast<const expressions::Predicate &>(pickUpEff0.argument());
	ASSERT_EQ(pickUpEff00.name(), "ontable");
	ASSERT_EQ(pickUpEff00.arguments().size(), 1u);
	const auto &pickUpEff000 = *dynamic_cast<const expressions::Reference<expressions::Variable> &>(*pickUpEff00.arguments()[0]).value();
	ASSERT_EQ(pickUpEff000.name(), "x");
	ASSERT_EQ(pickUpEff000.type(), &block);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseBlocksWorldProblem)
{
	const auto description = Description::fromFiles({"data/blocksworld-domain.pddl", "data/blocksworld-problem.pddl"});

	ASSERT_NO_THROW(description.problem());

	const auto &problem = description.problem();

	// Name
	ASSERT_EQ(problem.name(), "blocks-4-0");
	ASSERT_EQ(problem.domain().name(), "blocks");

	// Requirements
	// TODO: compute domain vs. problem requirements correctly and check them

	// Objects
	ASSERT_EQ(problem.objects().size(), 4u);

	ASSERT_EQ(problem.objects()[0]->name(), "d");
	ASSERT_NE(problem.objects()[0]->type(), nullptr);
	ASSERT_EQ(problem.objects()[0]->type()->name(), "block");
	ASSERT_EQ(problem.objects()[3]->name(), "c");
	ASSERT_NE(problem.objects()[3]->type(), nullptr);
	ASSERT_EQ(problem.objects()[3]->type()->name(), "block");

	// Initial State
	const auto &facts = problem.initialState().facts();

	ASSERT_EQ(facts.size(), 9u);
	const auto &fact0 = *dynamic_cast<const expressions::Predicate *>(facts[0].get());
	ASSERT_EQ(fact0.name(), "clear");
	ASSERT_EQ(fact0.arguments().size(), 1u);
	const auto &fact00 = *dynamic_cast<const expressions::Reference<expressions::Constant> *>(fact0.arguments()[0].get())->value();
	ASSERT_EQ(fact00.name(), "c");
	ASSERT_NE(fact00.type(), nullptr);
	ASSERT_EQ(fact00.type()->name(), "block");
	const auto &fact8 = *dynamic_cast<const expressions::Predicate *>(facts[8].get());
	ASSERT_EQ(fact8.name(), "handempty");
	ASSERT_EQ(fact8.arguments().size(), 0u);

	// Goal
	const auto &goal = dynamic_cast<const expressions::And &>(problem.goal());

	ASSERT_EQ(goal.arguments().size(), 3u);
	const auto &goal0 = *dynamic_cast<expressions::Predicate *>(goal.arguments()[0].get());
	ASSERT_EQ(goal0.name(), "on");
	ASSERT_EQ(goal0.arguments().size(), 2u);
	const auto &goal00 = *dynamic_cast<expressions::Reference<expressions::Constant> *>(goal0.arguments()[0].get())->value();
	ASSERT_EQ(goal00.name(), "d");
	const auto &goal01 = *dynamic_cast<expressions::Reference<expressions::Constant> *>(goal0.arguments()[1].get())->value();
	ASSERT_EQ(goal01.name(), "c");
	const auto &goal2 = *dynamic_cast<expressions::Predicate *>(goal.arguments()[2].get());
	ASSERT_EQ(goal2.name(), "on");
	ASSERT_EQ(goal2.arguments().size(), 2u);
	const auto &goal20 = *dynamic_cast<expressions::Reference<expressions::Constant> *>(goal2.arguments()[0].get())->value();
	ASSERT_EQ(goal20.name(), "b");
	const auto &goal21 = *dynamic_cast<expressions::Reference<expressions::Constant> *>(goal2.arguments()[1].get())->value();
	ASSERT_EQ(goal21.name(), "a");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseStorageDomain)
{
	const auto description = plasp::pddl::Description::fromFile("data/storage-domain.pddl");

	ASSERT_NO_THROW(description.domain());

	const auto &domain = description.domain();

	// Name
	ASSERT_EQ(domain.name(), "storage-propositional");

	// Requirements
	ASSERT_EQ(domain.requirements().size(), 1u);
	ASSERT_EQ(domain.requirements()[0].type(), Requirement::Type::Typing);

	// Types
	ASSERT_EQ(domain.types().size(), 10u);

	const auto &hoist = *domain.types()[0];
	const auto &surface = *domain.types()[1];
	const auto &area = *domain.types()[3];
	const auto &object = *domain.types()[4];
	const auto &storearea = *domain.types()[7];
	const auto &crate = *domain.types()[9];

	const auto &hoistParents = hoist.parentTypes();
	ASSERT_EQ(hoistParents.size(), 1u);
	ASSERT_TRUE(std::find(hoistParents.cbegin(), hoistParents.cend(), &object) != hoistParents.cend());

	const auto &areaParents = area.parentTypes();
	ASSERT_EQ(areaParents.size(), 2u);
	ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &object) != areaParents.cend());
	ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &surface) != areaParents.cend());

	// Predicates
	ASSERT_EQ(domain.predicates().size(), 8u);

	const auto &on = *domain.predicates()[5];

	ASSERT_EQ(on.name(), "on");
	ASSERT_EQ(on.arguments().size(), 2u);
	ASSERT_EQ(on.arguments()[0]->name(), "c");
	const auto onArgument0Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[0]->type());
	ASSERT_EQ(onArgument0Type, &crate);
	ASSERT_EQ(on.arguments()[1]->name(), "s");
	const auto onArgument1Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[1]->type());
	ASSERT_EQ(onArgument1Type, &storearea);

	const auto &in = *domain.predicates()[1];
	ASSERT_EQ(in.name(), "in");
	ASSERT_EQ(in.arguments().size(), 2u);
	ASSERT_EQ(in.arguments()[0]->name(), "x");
	const auto inArgument0Type = dynamic_cast<const expressions::Either *>(in.arguments()[0]->type());
	ASSERT_EQ(inArgument0Type->arguments().size(), 2u);
	const auto inArgument0Type0 = dynamic_cast<const expressions::Reference<expressions::PrimitiveType> *>(inArgument0Type->arguments()[0].get())->value();
	ASSERT_EQ(inArgument0Type0, &storearea);
	const auto inArgument0Type1 = dynamic_cast<const expressions::Reference<expressions::PrimitiveType> *>(inArgument0Type->arguments()[1].get())->value();
	ASSERT_EQ(inArgument0Type1, &crate);

	// Actions
	ASSERT_EQ(domain.actions().size(), 5u);

	const auto &drop = *domain.actions()[1];

	ASSERT_EQ(drop.name(), "drop");
	ASSERT_EQ(drop.parameters().size(), 5u);
	ASSERT_EQ(drop.parameters()[3]->name(), "a2");
	ASSERT_EQ(drop.parameters()[3]->type(), &area);

	const auto &dropPre = dynamic_cast<const expressions::And &>(drop.precondition());
	ASSERT_EQ(dropPre.arguments().size(), 5u);
	const auto &dropPre2 = dynamic_cast<const expressions::Predicate &>(*dropPre.arguments()[2]);
	ASSERT_EQ(dropPre2.name(), "lifting");
	ASSERT_EQ(dropPre2.arguments().size(), 2u);
	const auto &dropPre21 = *dynamic_cast<const expressions::Reference<expressions::Variable> &>(*dropPre2.arguments()[1]).value();
	ASSERT_EQ(dropPre21.name(), "c");
	ASSERT_EQ(dropPre21.type(), &crate);

	const auto &dropEff = dynamic_cast<const expressions::And &>(drop.effect());
	ASSERT_EQ(dropEff.arguments().size(), 5u);
	const auto &dropEff2 = dynamic_cast<const expressions::Not &>(*dropEff.arguments()[2]);
	const auto &dropEff20 = dynamic_cast<const expressions::Predicate &>(dropEff2.argument());
	ASSERT_EQ(dropEff20.name(), "clear");
	ASSERT_EQ(dropEff20.arguments().size(), 1u);
	const auto &dropEff200 = *dynamic_cast<const expressions::Reference<expressions::Variable> &>(*dropEff20.arguments()[0]).value();
	ASSERT_EQ(dropEff200.name(), "a1");
	ASSERT_EQ(dropEff200.type(), &storearea);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseStorageProblem)
{
	const auto description = Description::fromFiles({"data/storage-domain.pddl", "data/storage-problem.pddl"});

	ASSERT_NO_THROW(description.problem());

	const auto &problem = description.problem();

	// Name
	ASSERT_EQ(problem.name(), "storage-1");
	ASSERT_EQ(problem.domain().name(), "storage-propositional");

	// Requirements
	// TODO: compute domain vs. problem requirements correctly and check them

	// Objects
	ASSERT_EQ(problem.objects().size(), 7u);

	ASSERT_EQ(problem.objects()[0]->name(), "depot0-1-1");
	ASSERT_NE(problem.objects()[0]->type(), nullptr);
	ASSERT_EQ(problem.objects()[0]->type()->name(), "storearea");
	ASSERT_EQ(problem.objects()[6]->name(), "loadarea");
	ASSERT_NE(problem.objects()[6]->type(), nullptr);
	ASSERT_EQ(problem.objects()[6]->type()->name(), "transitarea");

	// Initial State
	const auto &facts = problem.initialState().facts();

	ASSERT_EQ(facts.size(), 10u);
	const auto &fact0 = *dynamic_cast<const expressions::Predicate *>(facts[0].get());
	ASSERT_EQ(fact0.name(), "in");
	ASSERT_EQ(fact0.arguments().size(), 2u);
	const auto &fact01 = *dynamic_cast<const expressions::Reference<expressions::Constant> *>(fact0.arguments()[1].get())->value();
	ASSERT_EQ(fact01.name(), "depot0");
	ASSERT_NE(fact01.type(), nullptr);
	ASSERT_EQ(fact01.type()->name(), "depot");
	const auto &fact9 = *dynamic_cast<const expressions::Predicate *>(facts[9].get());
	ASSERT_EQ(fact9.name(), "available");
	ASSERT_EQ(fact9.arguments().size(), 1u);
	const auto &fact90 = *dynamic_cast<const expressions::Reference<expressions::Constant> *>(fact9.arguments()[0].get())->value();
	ASSERT_EQ(fact90.name(), "hoist0");
	ASSERT_NE(fact90.type(), nullptr);
	ASSERT_EQ(fact90.type()->name(), "hoist");

	// TODO: check goal
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseConstants)
{
	const auto description = Description::fromFile("data/woodworking-domain.pddl");

	ASSERT_NO_THROW(description.domain());

	const auto &domain = description.domain();

	// Name
	ASSERT_EQ(domain.name(), "woodworking");

	// Types
	const auto &acolour = *domain.types()[0];
	const auto &surface = *domain.types()[4];
	const auto &treatmentstatus = *domain.types()[5];

	// Constants
	ASSERT_EQ(domain.constants().size(), 8u);
	ASSERT_EQ(domain.constants()[0]->type(), &surface);
	ASSERT_EQ(domain.constants()[2]->type(), &surface);
	ASSERT_EQ(domain.constants()[3]->type(), &treatmentstatus);
	ASSERT_EQ(domain.constants()[6]->type(), &treatmentstatus);
	ASSERT_EQ(domain.constants()[7]->type(), &acolour);

	// TODO: add test with constants in predicates
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, ParseWithWhiteSpace)
{
	ASSERT_NO_THROW(Description::fromFile("data/white-space-test.pddl"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, CheckWrongDomain)
{
	ASSERT_THROW(Description::fromFile("data/blocksworld-problem.pddl"), ConsistencyException);
	ASSERT_THROW(Description::fromFiles({"data/blocksworld-problem.pddl", "data/storage-domain.pddl"}), plasp::utils::ParserException);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, CheckSyntaxErrors)
{
	ASSERT_NO_THROW(Description::fromFile("data/pddl-syntax/domain-valid.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expressions-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expressions-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expressions-3.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expression-name-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expression-name-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-expression-name-3.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-3.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-4.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-5.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-6.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-7.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-parentheses-8.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-3.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-4.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-5.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-6.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-section-name-7.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-types-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-types-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-types-3.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-types-4.pddl"));

	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-variables-1.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-variables-2.pddl"));
	ASSERT_ANY_THROW(Description::fromFile("data/pddl-syntax/domain-variables-3.pddl"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLParserTests, CheckIssues)
{
	// Check white space issues with constants and parsing unsupported sections
	ASSERT_NO_THROW(Description::fromFile("data/issues/issue-1.pddl"));

	// Check white space issues with empty n-ary predicates
	ASSERT_NO_THROW(Description::fromFile("data/issues/issue-2.pddl"));
}
