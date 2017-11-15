#include <catch.hpp>

#include <experimental/filesystem>

#include <pddl/AST.h>
#include <pddl/Parse.h>

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

	// Check that accidentally unnamed variables lead to an exception and not a segfault
	SECTION("whitespace in typing section")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-9.pddl";
		context.tokenizer.read(domainFile);
		CHECK_THROWS(pddl::parseDescription(context));
	}

	// Check that “at” is allowed as a predicate name and not exclusively for “at” expressions
	SECTION("“at” as predicate name")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-10.pddl";
		context.tokenizer.read(domainFile);

		const auto description = pddl::parseDescription(context);

		REQUIRE(description.problem);

		const auto &problem = description.problem.value();
		const auto &facts = problem->initialState.facts;

		const auto invalidFact = std::find_if(facts.cbegin(), facts.cend(),
			[](const auto &fact)
			{
				return fact.template is<pddl::ast::AtPointer<pddl::ast::Literal>>();
			});

		const auto containsInvalidFact = (invalidFact != facts.cend());

		CHECK(!containsInvalidFact);
	}

	// Check that goal contains just one precondition and is correctly terminated by “)”
	SECTION("goal may contain only one precondition")
	{
		const auto domainFile = fs::path("data") / "issues" / "issue-11.pddl";
		context.tokenizer.read(domainFile);

		CHECK_THROWS(pddl::parseDescription(context));
	}
}
