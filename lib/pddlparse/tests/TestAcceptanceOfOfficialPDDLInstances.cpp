#include <catch.hpp>

#include <experimental/filesystem>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL acceptance] All official PDDL domains are parsed without errors", "[PDDL acceptance]")
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

			const auto testSectionName = competitionDirectory.path().stem().string() + ", "
				+ domainDirectory.path().stem().string();

			SECTION("domain [" + testSectionName + "]")
			{
				pddl::Tokenizer tokenizer;
				tokenizer.read(domainFile);
				pddl::Context context(std::move(tokenizer), ignoreWarnings, pddl::Mode::Compatibility);

				CHECK_NOTHROW(pddl::parseDescription(context));
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL acceptance] The first instance for all official PDDL domains is parsed without errors", "[PDDL acceptance]")
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

			const auto testSectionName = competitionDirectory.path().stem().string() + ", "
				+ domainDirectory.path().stem().string() + ", "
				+ instanceFile.stem().string();

			SECTION("instance [" + testSectionName + "]")
			{
				pddl::Tokenizer tokenizer;
				tokenizer.read(domainFile);
				tokenizer.read(instanceFile);
				pddl::Context context(std::move(tokenizer), ignoreWarnings, pddl::Mode::Compatibility);

				CHECK_NOTHROW(pddl::parseDescription(context));
			}
		}
	}
}
