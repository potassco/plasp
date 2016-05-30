#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <plasp/pddl/Description.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class PDDLParserTests : public ::testing::Test
{
	protected:
		PDDLParserTests()
		:	m_blocksworldDomainFile(readFile("data/blocksworld-domain.pddl"))
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

		std::stringstream m_blocksworldDomainFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseValidPDDLFile)
{
	try
	{
		const auto description = plasp::pddl::Description::fromStream(m_blocksworldDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		ASSERT_EQ(domain.requirements().size(), 2u);
		ASSERT_EQ(domain.requirements()[0], plasp::pddl::Requirement::Type::STRIPS);
		ASSERT_EQ(domain.requirements()[1], plasp::pddl::Requirement::Type::Typing);
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}
