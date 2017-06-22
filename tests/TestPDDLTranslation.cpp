#include <catch.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

#include <colorlog/Logger.h>

#include <pddlparse/AST.h>
#include <pddlparse/Parse.h>

#include <plasp/pddl/TranslatorASP.h>

boost::iostreams::stream<boost::iostreams::null_sink> nullStream((boost::iostreams::null_sink()));
const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL translation] Former issues are fixed", "[PDDL translation]")
{
	// TODO: refactor
	colorlog::Logger logger(nullStream, nullStream);
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("translating domains without typing information works")
	{
		context.tokenizer.read("data/issues/issue-4.pddl");
		auto description = pddl::parseDescription(context);
		const auto translator = plasp::pddl::TranslatorASP(std::move(description), logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}

	SECTION("translating the simple blocks world domain works")
	{
		context.tokenizer.read("data/issues/issue-5.pddl");
		auto description = pddl::parseDescription(context);
		const auto translator = plasp::pddl::TranslatorASP(std::move(description), logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}
}
