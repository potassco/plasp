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
		:	m_blocksworldDomainFile(readFile("data/blocksworld-domain.pddl")),
			m_storageDomainFile(readFile("data/storage-domain.pddl"))
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
		std::stringstream m_storageDomainFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseBlocksWorldDomain)
{
	try
	{
		const auto description = plasp::pddl::Description::fromStream(m_blocksworldDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		ASSERT_EQ(domain.name(), "BLOCKS");

		ASSERT_EQ(domain.requirements().size(), 2u);
		ASSERT_EQ(domain.requirements()[0].type(), plasp::pddl::Requirement::Type::STRIPS);
		ASSERT_EQ(domain.requirements()[1].type(), plasp::pddl::Requirement::Type::Typing);

		ASSERT_EQ(domain.types().size(), 1u);

		const auto block = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("block")->second);

		ASSERT_EQ(block.name(), "block");
		ASSERT_EQ(block.parentTypes().size(), 0u);

		ASSERT_EQ(domain.predicates().size(), 5u);

		const auto on = domain.predicates().find({"on", 2});
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseStorageDomain)
{
	try
	{
		const auto description = plasp::pddl::Description::fromStream(m_storageDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		ASSERT_EQ(domain.name(), "Storage-Propositional");

		ASSERT_EQ(domain.requirements().size(), 1u);
		ASSERT_EQ(domain.requirements()[0].type(), plasp::pddl::Requirement::Type::Typing);

		const auto area = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("area")->second);
		const auto hoist = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("hoist")->second);
		const auto object = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("object")->second);
		const auto storearea = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("storearea")->second);
		const auto surface = boost::get<plasp::pddl::TypePrimitive>(domain.types().find("surface")->second);

		const auto &hoistParents = hoist.parentTypes();
		ASSERT_EQ(hoistParents.size(), 1u);
		ASSERT_TRUE(std::find(hoistParents.cbegin(), hoistParents.cend(), &object) != hoistParents.cend());

		const auto &areaParents = area.parentTypes();
		ASSERT_EQ(areaParents.size(), 2u);
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &object) != areaParents.cend());
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &surface) != areaParents.cend());
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}
