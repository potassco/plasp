#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <plasp/sas/Description.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class SASParserTests : public ::testing::Test
{
	protected:
		SASParserTests()
		:	m_philosophersTestFile(readFile("data/philosophers.sas"))
		{
		}

		static std::stringstream readFile(const std::string &path)
		{
			std::ifstream fileStream(path, std::ios::in);

			std::stringstream outputStream;

			if (!fileStream.is_open())
				throw std::runtime_error("Could not open file \"" + path + "\"");

			outputStream << fileStream.rdbuf();

			return outputStream;
		}

		std::stringstream m_philosophersTestFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(SASParserTests, ParseValidSASFile)
{
	try
	{
		const auto description = plasp::sas::Description::fromStream(m_philosophersTestFile);

		ASSERT_FALSE(description.usesActionCosts());

		ASSERT_EQ(description.variables().size(), 37);
		ASSERT_EQ(description.variables()[0].axiomLayer(), -1);
		ASSERT_EQ(description.variables()[0].values()[0].sign(), plasp::sas::Value::Sign::Positive);
		ASSERT_EQ(description.variables()[0].values()[0].name(), "activate(philosopher-0, forks--pid-rfork)");
		ASSERT_EQ(description.variables()[36].axiomLayer(), -1);
		ASSERT_EQ(description.variables()[36].values()[1].sign(), plasp::sas::Value::Sign::Negative);
		ASSERT_EQ(description.variables()[36].values()[1].name(), "queue-tail-msg(forks-1-, fork)");

		ASSERT_EQ(description.mutexGroups().size(), 8);
		ASSERT_EQ(description.mutexGroups()[0].facts().size(), 9);
		ASSERT_EQ(&description.mutexGroups()[0].facts()[0].value(), &description.variables()[0].values()[0]);
		ASSERT_EQ(description.mutexGroups()[7].facts().size(), 2);
		ASSERT_EQ(&description.mutexGroups()[7].facts()[1].value(), &description.variables()[34].values()[1]);

		ASSERT_EQ(description.initialState().facts().size(), 37);
		ASSERT_EQ(&description.initialState().facts()[0].value(), &description.variables()[0].values()[8]);
		ASSERT_EQ(&description.initialState().facts()[36].value(), &description.variables()[36].values()[1]);

		ASSERT_EQ(description.goal().facts().size(), 2);
		ASSERT_EQ(&description.goal().facts()[0].value(), &description.variables()[6].values()[0]);
		ASSERT_EQ(&description.goal().facts()[1].value(), &description.variables()[7].values()[0]);

		ASSERT_EQ(description.operators().size(), 34);
		ASSERT_EQ(description.operators()[0].predicate().name, "activate-trans");
		ASSERT_EQ(description.operators()[0].predicate().arguments.size(), 5);
		ASSERT_EQ(description.operators()[0].predicate().arguments[0], "philosopher-0");
		ASSERT_EQ(description.operators()[0].predicate().arguments[4], "state-3");
		ASSERT_EQ(description.operators()[0].preconditions().size(), 3);
		ASSERT_EQ(&description.operators()[0].preconditions()[0].value(), &description.variables()[4].values()[4]);
		ASSERT_EQ(&description.operators()[0].preconditions()[1].value(), &description.variables()[16].values()[1]);
		ASSERT_EQ(&description.operators()[0].preconditions()[2].value(), &description.variables()[0].values()[8]);
		ASSERT_EQ(description.operators()[0].effects().size(), 1);
		ASSERT_EQ(description.operators()[0].effects()[0].conditions.size(), 0);
		ASSERT_EQ(&description.operators()[0].effects()[0].postcondition.value(), &description.variables()[0].values()[0]);
		ASSERT_EQ(description.operators()[33].predicate().name, "queue-write");
		ASSERT_EQ(description.operators()[33].predicate().arguments.size(), 4);
		ASSERT_EQ(description.operators()[33].predicate().arguments[0], "philosopher-1");
		ASSERT_EQ(description.operators()[33].predicate().arguments[3], "fork");
		ASSERT_EQ(description.operators()[33].preconditions().size(), 2);
		ASSERT_EQ(&description.operators()[33].preconditions()[0].value(), &description.variables()[1].values()[3]);
		ASSERT_EQ(&description.operators()[33].preconditions()[1].value(), &description.variables()[2].values()[2]);
		ASSERT_EQ(description.operators()[33].effects().size(), 3);
		ASSERT_EQ(description.operators()[33].effects()[0].conditions.size(), 0);
		ASSERT_EQ(&description.operators()[33].effects()[0].postcondition.value(), &description.variables()[1].values()[7]);
		ASSERT_EQ(&description.operators()[33].effects()[2].postcondition.value(), &description.variables()[35].values()[0]);

		ASSERT_EQ(description.axiomRules().size(), 33);
		ASSERT_EQ(description.axiomRules()[0].conditions().size(), 4);
		ASSERT_EQ(&description.axiomRules()[0].conditions()[0].value(), &description.variables()[0].values()[0]);
		ASSERT_EQ(&description.axiomRules()[0].conditions()[2].value(), &description.variables()[27].values()[0]);
		ASSERT_EQ(&description.axiomRules()[0].conditions()[3].value(), &description.variables()[8].values()[1]);
		ASSERT_EQ(&description.axiomRules()[0].postcondition().value(), &description.variables()[8].values()[0]);
		ASSERT_EQ(description.axiomRules()[32].conditions().size(), 2);
		ASSERT_EQ(&description.axiomRules()[32].conditions()[0].value(), &description.variables()[15].values()[0]);
		ASSERT_EQ(&description.axiomRules()[32].conditions()[1].value(), &description.variables()[25].values()[0]);
		ASSERT_EQ(&description.axiomRules()[32].postcondition().value(), &description.variables()[25].values()[1]);
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
