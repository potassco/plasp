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
	// Check that translating domains without typing information works
	{
		auto description = Description::fromFile("data/issues/issue-4.pddl");
		const auto translator = TranslatorASP(description, description.context().logger.outputStream());
		REQUIRE_NOTHROW(translator.translate());
	}

	// Check that translating the simple blocks world domain works
	{
		auto description = Description::fromFile("data/issues/issue-5.pddl");
		const auto translator = TranslatorASP(description, description.context().logger.outputStream());
		REQUIRE_NOTHROW(translator.translate());
	}
}
