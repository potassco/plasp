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

		const auto block = domain.types().find("block");
		ASSERT_NE(block, domain.types().cend());

		ASSERT_EQ(block->second.name(), "block");
		ASSERT_EQ(block->second.parentTypes().size(), 0u);

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

		const auto area = domain.types().find("area");
		ASSERT_NE(area, domain.types().cend());
		const auto hoist = domain.types().find("hoist");
		ASSERT_NE(hoist, domain.types().cend());
		const auto object = domain.types().find("object");
		ASSERT_NE(object, domain.types().cend());
		const auto storearea = domain.types().find("storearea");
		ASSERT_NE(storearea, domain.types().cend());
		const auto surface= domain.types().find("surface");
		ASSERT_NE(surface, domain.types().cend());

		const auto &hoistParents = hoist->second.parentTypes();
		ASSERT_EQ(hoistParents.size(), 1u);
		ASSERT_TRUE(std::find(hoistParents.cbegin(), hoistParents.cend(), &object->second) != hoistParents.cend());

		const auto &areaParents = area->second.parentTypes();
		ASSERT_EQ(areaParents.size(), 2u);
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &object->second) != areaParents.cend());
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &surface->second) != areaParents.cend());
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}
