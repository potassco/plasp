#include <catch.hpp>

#include <experimental/filesystem>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &warning){std::cout << warning << std::endl;};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser] Check past issues", "[PDDL parser]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	// Check that no infinite loop occurs
	SECTION("“either” in typing section")
	{
		const auto domainFile = fs::path("data") / "test-cases" / "typing-1.pddl";
		context.tokenizer.read(domainFile);
		const auto description = pddl::parseDescription(context);

		const auto &types = description.domain->types;

		REQUIRE(types.size() == 5);
		CHECK(types[0]->name == "object");
		REQUIRE(types[0]->parentTypes.size() == 1);
		CHECK(types[0]->parentTypes[0]->declaration == types[0].get());
		CHECK(types[1]->name == "a1");
		REQUIRE(types[1]->parentTypes.size() == 1);
		CHECK(types[1]->parentTypes[0]->declaration == types[0].get());
		CHECK(types[2]->name == "a2");
		REQUIRE(types[2]->parentTypes.size() == 1);
		CHECK(types[2]->parentTypes[0]->declaration == types[0].get());
		CHECK(types[3]->name == "a3");
		REQUIRE(types[3]->parentTypes.size() == 1);
		CHECK(types[3]->parentTypes[0]->declaration == types[0].get());
		CHECK(types[4]->name == "bx");
		REQUIRE(types[4]->parentTypes.size() == 3);
		CHECK(types[4]->parentTypes[0]->declaration == types[1].get());
		CHECK(types[4]->parentTypes[1]->declaration == types[2].get());
		CHECK(types[4]->parentTypes[2]->declaration == types[3].get());
	}

	// Check that whitespace is handled appropriately
	SECTION("“either” in typing section")
	{
		const auto domainFile = fs::path("data") / "test-cases" / "white-space.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}
}
