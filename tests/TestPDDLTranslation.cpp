#include <catch.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

#include <plasp/pddl/Description.h>
#include <plasp/pddl/TranslatorASP.h>

using namespace plasp::pddl;

boost::iostreams::stream<boost::iostreams::null_sink> nullStream((boost::iostreams::null_sink()));

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL translation] Former issues are fixed", "[PDDL translation]")
{
	plasp::output::Logger logger;
	Context context(Tokenizer(), logger);

	SECTION("translating domains without typing information works")
	{
		auto description = Description::fromFile("data/issues/issue-4.pddl", context);
		const auto translator = TranslatorASP(description, description.context().logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}

	SECTION("translating the simple blocks world domain works")
	{
		auto description = Description::fromFile("data/issues/issue-5.pddl", context);
		const auto translator = TranslatorASP(description, description.context().logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}
}
