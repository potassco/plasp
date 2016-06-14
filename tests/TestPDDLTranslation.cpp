#include <gtest/gtest.h>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

#include <plasp/pddl/Description.h>
#include <plasp/pddl/TranslatorASP.h>

using namespace plasp::pddl;

boost::iostreams::stream<boost::iostreams::null_sink> nullStream((boost::iostreams::null_sink()));

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(PDDLTranslationTests, CheckIssues)
{
	// Check that translating domains without typing information works
	auto description = Description::fromFile("data/issues/issue-4.pddl");
	const auto translator = TranslatorASP(description, description.context().logger.outputStream());
	ASSERT_NO_THROW(translator.translate());
}
