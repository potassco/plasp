#include <catch.hpp>

#include <experimental/filesystem>
#include <set>

#include <pddl/AST.h>
#include <pddl/Parse.h>

namespace fs = std::experimental::filesystem;

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};
const auto pddlInstanceBasePath = fs::path("data") / "pddl-instances";

const std::set<std::experimental::filesystem::path> unsupportedDomains =
	{
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-numeric-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-time-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-time-hand-coded" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-time-simple-automatic" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "depots-time-simple-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-numeric-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-numeric-hard-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-numeric-hard-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-time-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-time-hand-coded" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-time-simple-automatic" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "driverlog-time-simple-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-numeric-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-time-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-time-hand-coded" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-time-simple-automatic" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "rovers-time-simple-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-complex-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-complex-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-numeric-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-numeric-hard-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-time-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-time-hand-coded" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-time-simple-automatic" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "satellite-time-simple-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "settlers-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "umtranslog-2-numeric-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-numeric-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-numeric-hand-coded" / "domain.pddl",
		// “either” expressions unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-strips-automatic" / "domain.pddl",
		// “either” expressions unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-strips-hand-coded" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-time-automatic" / "domain.pddl",
		// “:functions” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-time-hand-coded" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-time-simple-automatic" / "domain.pddl",
		// “:durative-action” sections unsupported
		pddlInstanceBasePath / "ipc-2002" / "domains" / "zenotravel-time-simple-hand-coded" / "domain.pddl",

	};

const std::set<std::experimental::filesystem::path> unsupportedInstances =
	{
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[parser success] All official PDDL domains are parsed without errors", "[parser success]")
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

			if (unsupportedDomains.find(domainFile) != unsupportedDomains.cend())
				continue;

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

TEST_CASE("[parser success] The first instance for all official PDDL domains is parsed without errors", "[parser success]")
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

			if (unsupportedDomains.find(domainFile) != unsupportedDomains.cend()
				|| unsupportedInstances.find(instanceFile) != unsupportedInstances.cend())
			{
				continue;
			}

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
