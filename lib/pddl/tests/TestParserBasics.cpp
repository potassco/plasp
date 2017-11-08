#include <catch.hpp>

#include <experimental/filesystem>

#include <pddl/AST.h>
#include <pddl/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser basics] The PDDL parser behaves correctly", "[parser basics]")
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

	SECTION("missing domains are detected")
	{
		const auto instanceFile = fs::path("data") / "pddl-instances" / "ipc-2000" / "domains" / "blocks-strips-typed" / "instances" / "instance-1.pddl";
		context.tokenizer.read(instanceFile);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	SECTION("mismatched domains are detected")
	{
		const auto domainFile = fs::path("data") / "pddl-instances" / "ipc-2000" / "domains" / "blocks-strips-typed" / "domain.pddl";
		const auto instanceFile = fs::path("data") / "pddl-instances" / "ipc-2000" / "domains" / "freecell-strips-typed" / "instances" / "instance-1.pddl";
		context.tokenizer.read(domainFile);
		context.tokenizer.read(instanceFile);
		CHECK_THROWS(pddl::parseDescription(context));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser basics] Syntax errors are correctly recognized", "[parser basics]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("valid domain")
	{
		const auto domainFile = fs::path("data") / "pddl-syntax" / "domain-valid.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	for (size_t i = 1; i <= 3; i++)
		SECTION("syntax errors in expressions (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-expressions-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}

	for (size_t i = 1; i <= 3; i++)
		SECTION("syntax errors in expression names (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-expression-name-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}

	for (size_t i = 1; i <= 8; i++)
		SECTION("syntax errors with parentheses (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-parentheses-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}

	for (size_t i = 1; i <= 7; i++)
		SECTION("syntax errors in section names (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-section-name-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}

	for (size_t i = 1; i <= 4; i++)
		SECTION("syntax errors in types (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-types-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}

	for (size_t i = 1; i <= 3; i++)
		SECTION("syntax errors in variables (" + std::to_string(i) + ")")
		{
			const auto domainFile = fs::path("data") / "pddl-syntax" / ("domain-variables-" + std::to_string(i) + ".pddl");
			context.tokenizer.read(domainFile);
			CHECK_THROWS(pddl::parseDescription(context));
		}
}
