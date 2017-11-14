#include <catch.hpp>

#include <experimental/filesystem>

#include <pddl/AST.h>
#include <pddl/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser correctness] The official PDDL instances are parsed correctly", "[parser correctness]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("types, predicates, and actions in blocksworld domain")
	{
		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "blocks-strips-typed" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		CHECK(description.domain->name == "blocks");
		CHECK(description.domain->constants.empty());

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
		const auto &effect0 = effectAnd->arguments[0].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effect0->arguments.size() == 1);
		CHECK(effect0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effect0->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effect1 = effectAnd->arguments[1].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effect1->arguments.size() == 1);
		CHECK(effect1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "y");
		CHECK(effect1->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effectNot2 = effectAnd->arguments[2].get<pddl::ast::Literal>().get<pddl::ast::NotPointer<pddl::ast::AtomicFormula>>()->argument.get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot2->arguments.size() == 1);
		CHECK(effectNot2->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effectNot2->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &effectNot3 = effectAnd->arguments[3].get<pddl::ast::Literal>().get<pddl::ast::NotPointer<pddl::ast::AtomicFormula>>()->argument.get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot3->arguments.empty());
		const auto &effectNot4 = effectAnd->arguments[4].get<pddl::ast::Literal>().get<pddl::ast::NotPointer<pddl::ast::AtomicFormula>>()->argument.get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(effectNot4->arguments.size() == 2);
		CHECK(effectNot4->arguments[0].get<pddl::ast::VariablePointer>()->declaration->name == "x");
		CHECK(effectNot4->arguments[0].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(effectNot4->arguments[1].get<pddl::ast::VariablePointer>()->declaration->name == "y");
		CHECK(effectNot4->arguments[1].get<pddl::ast::VariablePointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
	}

	SECTION("types, predicates, and actions in blocksworld instance")
	{
		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "blocks-strips-typed" / "domain.pddl";
		const auto instanceFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "blocks-strips-typed" / "instances" / "instance-1.pddl";
		context.tokenizer.read(domainFile);
		context.tokenizer.read(instanceFile);
		auto description = pddl::parseDescription(context);

		const auto &types = description.domain->types;
		const auto &typeBlock = types[0];

		REQUIRE(description.problem);

		const auto &problem = description.problem.value();

		CHECK(problem->name == "blocks-4-0");
		CHECK(problem->domain->name == "blocks");

		const auto &objects = problem->objects;

		REQUIRE(objects.size() == 4);
		CHECK(objects[0]->name == "d");
		CHECK(objects[1]->name == "b");
		CHECK(objects[2]->name == "a");
		CHECK(objects[3]->name == "c");

		const auto &facts = problem->initialState.facts;

		REQUIRE(facts.size() == 9);
		const auto &fact0 = facts[0].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(fact0->arguments.size() == 1);
		CHECK(fact0->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "c");
		CHECK(fact0->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &fact5 = facts[5].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(fact5->arguments.size() == 1);
		CHECK(fact5->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "a");
		CHECK(fact5->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &fact8 = facts[8].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(fact8->arguments.empty());

		REQUIRE(problem->goal);

		const auto &goal = problem->goal.value();
		const auto &goalAnd = goal.get<pddl::ast::AndPointer<pddl::ast::Precondition>>();

		REQUIRE(goalAnd->arguments.size() == 3);
		const auto &goal0 = goalAnd->arguments[0].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(goal0->arguments.size() == 2);
		CHECK(goal0->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "d");
		CHECK(goal0->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(goal0->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->name == "c");
		CHECK(goal0->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &goal1 = goalAnd->arguments[1].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(goal0->arguments.size() == 2);
		CHECK(goal1->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "c");
		CHECK(goal1->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(goal1->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->name == "b");
		CHECK(goal1->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		const auto &goal2 = goalAnd->arguments[2].get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>();
		// TODO: check declaration once implemented
		REQUIRE(goal0->arguments.size() == 2);
		CHECK(goal2->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "b");
		CHECK(goal2->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
		CHECK(goal2->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->name == "a");
		CHECK(goal2->arguments[1].get<pddl::ast::ConstantPointer>()->declaration->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration == typeBlock.get());
	}

	SECTION("“either” type in zenotravel domain")
	{
		context.mode = pddl::Mode::Compatibility;

		const auto domainFile = pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-strips-hand-coded" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &predicates = description.domain->predicates;

		REQUIRE(predicates.size() == 4);
		REQUIRE(predicates[0]->name == "at");
		REQUIRE(predicates[0]->parameters.size() == 2);
		REQUIRE(predicates[0]->parameters[0]->name == "x");
		REQUIRE(predicates[0]->parameters[0]->type);
		CHECK(predicates[0]->parameters[0]->type.value().get<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>()->arguments[0]->declaration->name == "person");
		CHECK(predicates[0]->parameters[0]->type.value().get<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>()->arguments[1]->declaration->name == "aircraft");
		REQUIRE(predicates[0]->parameters[1]->name == "c");
		REQUIRE(predicates[0]->parameters[1]->type);
		CHECK(predicates[0]->parameters[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "city");
	}

	SECTION("typed constants in schedule domain")
	{
		context.mode = pddl::Mode::Compatibility;

		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "schedule-adl-typed" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &constants = description.domain->constants;

		REQUIRE(constants.size() == 14);
		CHECK(constants[0]->name == "cold");
		CHECK(constants[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "temperature");
		CHECK(constants[1]->name == "hot");
		CHECK(constants[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "temperature");
		CHECK(constants[2]->name == "cylindrical");
		CHECK(constants[2]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "ashape");
		CHECK(constants[3]->name == "polisher");
		CHECK(constants[3]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "machine");
		CHECK(constants[4]->name == "roller");
		CHECK(constants[4]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "machine");
		CHECK(constants[10]->name == "immersion-painter");
		CHECK(constants[10]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "machine");
		CHECK(constants[11]->name == "polished");
		CHECK(constants[11]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "surface");
		CHECK(constants[13]->name == "smooth");
		CHECK(constants[13]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "surface");
	}

	SECTION("type inheritance in logistics domain")
	{
		context.mode = pddl::Mode::Compatibility;

		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "logistics-strips-typed" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &types = description.domain->types;

		REQUIRE(types.size() == 10);
		CHECK(types[0]->name == "truck");
		REQUIRE(types[0]->parentTypes.size() == 1);
		CHECK(types[0]->parentTypes[0]->declaration->name == "vehicle");
		CHECK(types[1]->name == "airplane");
		REQUIRE(types[1]->parentTypes.size() == 1);
		CHECK(types[1]->parentTypes[0]->declaration->name == "vehicle");
		CHECK(types[2]->name == "vehicle");
		REQUIRE(types[2]->parentTypes.size() == 1);
		CHECK(types[2]->parentTypes[0]->declaration->name == "physobj");
		CHECK(types[3]->name == "package");
		REQUIRE(types[3]->parentTypes.size() == 1);
		CHECK(types[3]->parentTypes[0]->declaration->name == "physobj");
		CHECK(types[4]->name == "physobj");
		REQUIRE(types[4]->parentTypes.size() == 1);
		CHECK(types[4]->parentTypes[0]->declaration->name == "object");
		CHECK(types[5]->name == "airport");
		REQUIRE(types[5]->parentTypes.size() == 1);
		CHECK(types[5]->parentTypes[0]->declaration->name == "place");
		CHECK(types[6]->name == "location");
		REQUIRE(types[6]->parentTypes.size() == 1);
		CHECK(types[6]->parentTypes[0]->declaration->name == "place");
		CHECK(types[7]->name == "place");
		REQUIRE(types[7]->parentTypes.size() == 1);
		CHECK(types[7]->parentTypes[0]->declaration->name == "object");
		CHECK(types[8]->name == "city");
		REQUIRE(types[8]->parentTypes.size() == 1);
		CHECK(types[8]->parentTypes[0]->declaration->name == "object");
		CHECK(types[9]->name == "object");
		REQUIRE(types[9]->parentTypes.empty());
	}

	SECTION("typing in mystery domain")
	{
		context.mode = pddl::Mode::Compatibility;

		const auto domainFile = pddlInstanceBasePath / "ipc-1998" / "domains" / "mystery-round-1-adl" / "domain.pddl";
		const auto instanceFile = pddlInstanceBasePath / "ipc-1998" / "domains" / "mystery-round-1-adl" / "instances" / "instance-1.pddl";
		context.tokenizer.read(domainFile);
		context.tokenizer.read(instanceFile);
		auto description = pddl::parseDescription(context);

		const auto &actions = description.domain->actions;

		REQUIRE(actions.size() == 3);
		// TODO: adjust if there are changes to handling :vars section
		REQUIRE(actions[0]->parameters.size() == 5);
		CHECK(actions[0]->parameters[0]->name == "c");
		CHECK(actions[0]->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pain");
		CHECK(actions[0]->parameters[1]->name == "v");
		CHECK(actions[0]->parameters[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pleasure");
		CHECK(actions[0]->parameters[2]->name == "n");
		CHECK(actions[0]->parameters[2]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "food");
		CHECK(actions[0]->parameters[3]->name == "s1");
		CHECK(actions[0]->parameters[3]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "planet");
		CHECK(actions[0]->parameters[4]->name == "s2");
		CHECK(actions[0]->parameters[4]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "planet");

		REQUIRE(description.problem);

		const auto &problem = description.problem.value();
		const auto &objects = problem->objects;

		REQUIRE(objects.size() == 21);
		CHECK(objects[0]->name == "rice");
		CHECK(objects[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "food");
		CHECK(objects[1]->name == "pear");
		CHECK(objects[1]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "food");
		CHECK(objects[2]->name == "flounder");
		CHECK(objects[2]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "food");
		CHECK(objects[5]->name == "lamb");
		CHECK(objects[5]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "food");
		CHECK(objects[6]->name == "rest");
		CHECK(objects[6]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pleasure");
		CHECK(objects[7]->name == "hangover");
		CHECK(objects[7]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pain");
		CHECK(objects[8]->name == "depression");
		CHECK(objects[8]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pain");
		CHECK(objects[9]->name == "abrasion");
		CHECK(objects[9]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "pain");
		CHECK(objects[10]->name == "kentucky");
		CHECK(objects[10]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "province");
		CHECK(objects[16]->name == "guanabara");
		CHECK(objects[16]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "province");
		CHECK(objects[17]->name == "mars");
		CHECK(objects[17]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "planet");
		CHECK(objects[20]->name == "venus");
		CHECK(objects[20]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "planet");
	}

	SECTION("complicated nested effect expressions in schedule domain")
	{
		context.mode = pddl::Mode::Compatibility;

		const auto domainFile = pddlInstanceBasePath / "ipc-2000" / "domains" / "schedule-adl-typed" / "domain.pddl";
		context.tokenizer.read(domainFile);
		auto description = pddl::parseDescription(context);

		const auto &actions = description.domain->actions;

		REQUIRE(actions.size() == 9);
		CHECK(actions[1]->name == "do-roll");
		const auto &effectAnd = actions[1]->effect.value().get<pddl::ast::AndPointer<pddl::ast::Effect>>();
		REQUIRE(effectAnd->arguments.size() == 10);
		CHECK(effectAnd->arguments[0].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().get<pddl::ast::PredicatePointer>()->arguments[0].get<pddl::ast::ConstantPointer>()->declaration->name == "roller");
		CHECK(effectAnd->arguments[1].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		CHECK(effectAnd->arguments[2].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		CHECK(effectAnd->arguments[3].get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		const auto &effectWhen4 = effectAnd->arguments[4].get<pddl::ast::WhenPointer<pddl::ast::Precondition, pddl::ast::ConditionalEffect>>();
		// TODO: check name of declaration
		CHECK(effectWhen4->argumentLeft.get<pddl::ast::NotPointer<pddl::ast::Precondition>>()->argument.get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		CHECK(effectWhen4->argumentRight.get<pddl::ast::Literal>().get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		const auto &effectForAll5 = effectAnd->arguments[5].get<pddl::ast::ForAllPointer<pddl::ast::Effect>>();
		REQUIRE(effectForAll5->parameters.size() == 1);
		CHECK(effectForAll5->parameters[0]->name == "oldsurface");
		CHECK(effectForAll5->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "surface");
		const auto &effectForAll5When = effectForAll5->argument.get<pddl::ast::WhenPointer<pddl::ast::Precondition, pddl::ast::ConditionalEffect>>();
		// TODO: check name of declaration
		CHECK(effectForAll5When->argumentLeft.get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		CHECK(effectForAll5When->argumentRight.get<pddl::ast::Literal>().get<pddl::ast::NotPointer<pddl::ast::AtomicFormula>>()->argument.is<pddl::ast::PredicatePointer>());
		const auto &effectForAll6 = effectAnd->arguments[6].get<pddl::ast::ForAllPointer<pddl::ast::Effect>>();
		REQUIRE(effectForAll6->parameters.size() == 1);
		CHECK(effectForAll6->parameters[0]->name == "oldpaint");
		CHECK(effectForAll6->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "colour");
		const auto &effectForAll9 = effectAnd->arguments[9].get<pddl::ast::ForAllPointer<pddl::ast::Effect>>();
		REQUIRE(effectForAll9->parameters.size() == 1);
		CHECK(effectForAll9->parameters[0]->name == "oldtemp");
		CHECK(effectForAll9->parameters[0]->type.value().get<pddl::ast::PrimitiveTypePointer>()->declaration->name == "temperature");
		const auto &effectForAll9When = effectForAll9->argument.get<pddl::ast::WhenPointer<pddl::ast::Precondition, pddl::ast::ConditionalEffect>>();
		// TODO: check name of declaration
		CHECK(effectForAll9When->argumentLeft.get<pddl::ast::AtomicFormula>().is<pddl::ast::PredicatePointer>());
		CHECK(effectForAll9When->argumentRight.get<pddl::ast::Literal>().get<pddl::ast::NotPointer<pddl::ast::AtomicFormula>>()->argument.is<pddl::ast::PredicatePointer>());
	}
}
