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
	std::cout << std::endl;

	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

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
		CHECK(predicates[0]->parameters[0]->type.value().is<pddl::ast::EitherPointer<pddl::ast::PrimitiveTypePointer>>());
		REQUIRE(predicates[0]->parameters[1]->name == "c");
		REQUIRE(predicates[0]->parameters[1]->type);
		CHECK(predicates[0]->parameters[1]->type.value().is<pddl::ast::PrimitiveTypePointer>());
	}
}
