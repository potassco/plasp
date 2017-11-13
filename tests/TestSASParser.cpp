#include <catch.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <plasp/sas/Description.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class SASParserTestsFixture
{
	protected:
		SASParserTestsFixture()
		:	m_blocksworldTestFile(readFile("data/blocksworld.sas")),
			m_cavedivingTestFile(readFile("data/cavediving.sas")),
			m_freecellTestFile(readFile("data/freecell.sas")),
			m_philosophersTestFile(readFile("data/philosophers.sas"))
		{
		}

		static std::stringstream readFile(const std::string &path)
		{
			std::ifstream fileStream(path, std::ios::in);

			std::stringstream outputStream;

			if (!fileStream.is_open())
				throw std::runtime_error("Could not open file “" + path + "”");

			outputStream << fileStream.rdbuf();

			return outputStream;
		}

		std::stringstream m_blocksworldTestFile;
		std::stringstream m_cavedivingTestFile;
		std::stringstream m_freecellTestFile;
		std::stringstream m_philosophersTestFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE_METHOD(SASParserTestsFixture, "[SAS parser] A valid SAS file is parsed correctly", "[SAS parser]")
{
	try
	{
		const auto description = plasp::sas::Description::fromStream(m_philosophersTestFile);

		CHECK_FALSE(description.usesActionCosts());

		REQUIRE(description.variables().size() == 37u);
		CHECK(description.variables()[0].axiomLayer() == -1);
		CHECK(description.variables()[0].values()[0].sign() == plasp::sas::Value::Sign::Positive);
		CHECK(description.variables()[0].values()[0].name() == "activate(philosopher-0, forks--pid-rfork)");
		CHECK(description.variables()[36].axiomLayer() == -1);
		CHECK(description.variables()[36].values()[1].sign() == plasp::sas::Value::Sign::Negative);
		CHECK(description.variables()[36].values()[1].name() == "queue-tail-msg(forks-1-, fork)");

		REQUIRE(description.mutexGroups().size() == 8u);
		REQUIRE(description.mutexGroups()[0].facts().size() == 9u);
		CHECK(&description.mutexGroups()[0].facts()[0].value() == &description.variables()[0].values()[0]);
		REQUIRE(description.mutexGroups()[7].facts().size() == 2u);
		CHECK(&description.mutexGroups()[7].facts()[1].value() == &description.variables()[34].values()[1]);

		REQUIRE(description.initialState().facts().size() == 37u);
		CHECK(&description.initialState().facts()[0].value() == &description.variables()[0].values()[8]);
		CHECK(&description.initialState().facts()[36].value() == &description.variables()[36].values()[1]);

		REQUIRE(description.goal().facts().size() == 2u);
		CHECK(&description.goal().facts()[0].value() == &description.variables()[6].values()[0]);
		CHECK(&description.goal().facts()[1].value() == &description.variables()[7].values()[0]);

		REQUIRE(description.operators().size() == 34u);
		CHECK(description.operators()[0].predicate().name() == "activate-trans");
		REQUIRE(description.operators()[0].predicate().arguments().size() == 5u);
		CHECK(description.operators()[0].predicate().arguments()[0] == "philosopher-0");
		CHECK(description.operators()[0].predicate().arguments()[4] == "state-3");
		REQUIRE(description.operators()[0].preconditions().size() == 3u);
		CHECK(&description.operators()[0].preconditions()[0].value() == &description.variables()[4].values()[4]);
		CHECK(&description.operators()[0].preconditions()[1].value() == &description.variables()[16].values()[1]);
		CHECK(&description.operators()[0].preconditions()[2].value() == &description.variables()[0].values()[8]);
		REQUIRE(description.operators()[0].effects().size() == 1u);
		REQUIRE(description.operators()[0].effects()[0].conditions().size() == 0u);
		CHECK(&description.operators()[0].effects()[0].postcondition().value() == &description.variables()[0].values()[0]);
		CHECK(description.operators()[33].predicate().name() == "queue-write");
		REQUIRE(description.operators()[33].predicate().arguments().size() == 4u);
		CHECK(description.operators()[33].predicate().arguments()[0] == "philosopher-1");
		CHECK(description.operators()[33].predicate().arguments()[3] == "fork");
		REQUIRE(description.operators()[33].preconditions().size() == 2u);
		CHECK(&description.operators()[33].preconditions()[0].value() == &description.variables()[1].values()[3]);
		CHECK(&description.operators()[33].preconditions()[1].value() == &description.variables()[2].values()[2]);
		REQUIRE(description.operators()[33].effects().size() == 3u);
		REQUIRE(description.operators()[33].effects()[0].conditions().size() == 0u);
		CHECK(&description.operators()[33].effects()[0].postcondition().value() == &description.variables()[1].values()[7]);
		CHECK(&description.operators()[33].effects()[2].postcondition().value() == &description.variables()[35].values()[0]);

		REQUIRE(description.axiomRules().size() == 33u);
		REQUIRE(description.axiomRules()[0].conditions().size() == 4u);
		CHECK(&description.axiomRules()[0].conditions()[0].value() == &description.variables()[0].values()[0]);
		CHECK(&description.axiomRules()[0].conditions()[2].value() == &description.variables()[27].values()[0]);
		CHECK(&description.axiomRules()[0].conditions()[3].value() == &description.variables()[8].values()[1]);
		CHECK(&description.axiomRules()[0].postcondition().value() == &description.variables()[8].values()[0]);
		REQUIRE(description.axiomRules()[32].conditions().size() == 2u);
		CHECK(&description.axiomRules()[32].conditions()[0].value() == &description.variables()[15].values()[0]);
		CHECK(&description.axiomRules()[32].conditions()[1].value() == &description.variables()[25].values()[0]);
		CHECK(&description.axiomRules()[32].postcondition().value() == &description.variables()[25].values()[1]);
	}
	catch (const std::exception &e)
	{
		FAIL(e.what());
	}

	// TODO: add whitespace test
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE_METHOD(SASParserTestsFixture, "[SAS parser] Trailing empty parentheses are removed", "[SAS parser]")
{
	try
	{
		const auto description = plasp::sas::Description::fromStream(m_blocksworldTestFile);

		CHECK(description.variables()[4].values()[0].name() == "handempty");
		CHECK(description.variables()[5].values()[0].name() == "holding(a)");
	}
	catch (const std::exception &e)
	{
		FAIL(e.what());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE_METHOD(SASParserTestsFixture, "[SAS parser] “none” values are correctly parsed", "[SAS parser]")
{
	try
	{
		const auto description = plasp::sas::Description::fromStream(m_freecellTestFile);

		// TODO: compare by identity, not value
		CHECK(description.variables()[0].values()[3] == plasp::sas::Value::None);
		CHECK(description.variables()[5].values()[6] == plasp::sas::Value::None);
	}
	catch (const std::exception &e)
	{
		FAIL(e.what());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE_METHOD(SASParserTestsFixture, "[SAS parser] SAS requirements are parsed correctly", "[SAS parser]")
{
	try
	{
		const auto description = plasp::sas::Description::fromStream(m_cavedivingTestFile);

		CHECK(description.usesActionCosts());
		CHECK(description.usesConditionalEffects());
		CHECK_FALSE(description.usesAxiomRules());

		REQUIRE(description.operators().size() == 496u);

		CHECK(description.operators()[0].costs() == 1u);
		CHECK(description.operators()[172].costs() == 10u);
		CHECK(description.operators()[173].costs() == 63u);

		REQUIRE(description.operators()[172].effects().size() == 3u);
		REQUIRE(description.operators()[172].effects()[1].conditions().size() == 1u);
		CHECK(&description.operators()[172].effects()[1].conditions()[0].value() == &description.variables()[1].values()[4]);
	}
	catch (const std::exception &e)
	{
		FAIL(e.what());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[SAS parser] Former issues are fixed", "[SAS parser]")
{
	// Check issue where unexpected whitespaces in SAS files led to a parsing error
	CHECK_NOTHROW(plasp::sas::Description::fromFile("data/issues/issue-6.sas"));
}
