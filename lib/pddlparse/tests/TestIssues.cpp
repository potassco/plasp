#include <catch.hpp>

#include <experimental/filesystem>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser issues] Check past issues", "[PDDL parser issues]")
{
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("white space issues with constants and parsing unsupported sections")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-1.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("white space issues with empty n-ary predicates")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-2.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	SECTION("comments are correctly ignored")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-3.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}

	// Check that no infinite loop occurs
	SECTION("“either” in typing section")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-7.pddl";
		context.tokenizer.read(domainFile);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	// Check that whitespace is correctly ignored in type section
	SECTION("whitespace in typing section")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-8.pddl";
		context.tokenizer.read(domainFile);
		CHECK_NOTHROW(pddl::parseDescription(context));
	}
}
