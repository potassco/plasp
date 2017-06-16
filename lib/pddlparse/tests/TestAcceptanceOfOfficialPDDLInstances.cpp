#include <catch.hpp>

#include <experimental/filesystem>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser acceptance] All official PDDL domains are parsed without errors", "[PDDL parser acceptance]")
{
	for (const auto &competitionDirectory : fs::directory_iterator(pddlInstanceBasePath))
	{
		if (!fs::is_directory(competitionDirectory))
			continue;

		for (const auto &domainDirectory : fs::directory_iterator(competitionDirectory.path() / "domains"))
		{
			if (!fs::is_directory(domainDirectory))
				continue;

			const auto domainFile = domainDirectory / "domain.pddl";

			pddl::Tokenizer tokenizer;
			tokenizer.read(domainFile);
			pddl::Context context(std::move(tokenizer), ignoreWarnings);

			CHECK_NOTHROW(pddl::parseDescription(context));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL parser acceptance] The first instance for all official PDDL domains is parsed without errors", "[PDDL parser acceptance]")
{
	for (const auto &competitionDirectory : fs::directory_iterator(pddlInstanceBasePath))
	{
		if (!fs::is_directory(competitionDirectory))
			continue;

		for (const auto &domainDirectory : fs::directory_iterator(competitionDirectory.path() / "domains"))
		{
			if (!fs::is_directory(domainDirectory))
				continue;

			const auto domainFile = domainDirectory / "domain.pddl";
			const auto instanceFile = domainDirectory / "instances" / "instance-1.pddl";

			pddl::Tokenizer tokenizer;
			tokenizer.read(domainFile);
			tokenizer.read(instanceFile);
			pddl::Context context(std::move(tokenizer), ignoreWarnings);

			CHECK_NOTHROW(pddl::parseDescription(context));
		}
	}
}
