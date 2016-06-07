#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <plasp/pddl/Description.h>
#include <plasp/pddl/expressions/Either.h>
#include <plasp/pddl/expressions/PrimitiveType.h>
#include <plasp/pddl/expressions/Reference.h>

////////////////////////////////////////////////////////////////////////////////////////////////////

class PDDLParserTests : public ::testing::Test
{
	protected:
		PDDLParserTests()
		:	m_blocksworldDomainFile(readFile("data/blocksworld-domain.pddl")),
			m_storageDomainFile(readFile("data/storage-domain.pddl")),
			m_whiteSpaceTestFile(readFile("data/white-space-test.pddl")),
			m_woodworkingDomainFile(readFile("data/woodworking-domain.pddl"))
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
		std::stringstream m_whiteSpaceTestFile;
		std::stringstream m_woodworkingDomainFile;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseBlocksWorldDomain)
{
	using namespace plasp::pddl;

	try
	{
		const auto description = Description::fromStream(m_blocksworldDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		// Name
		ASSERT_EQ(domain.name(), "blocks");

		// Requirements
		ASSERT_EQ(domain.requirements().size(), 2u);
		ASSERT_EQ(domain.requirements()[0].type(), Requirement::Type::STRIPS);
		ASSERT_EQ(domain.requirements()[1].type(), Requirement::Type::Typing);

		// Types
		ASSERT_EQ(domain.types().size(), 1u);

		const auto &block = *domain.types()[0];

		ASSERT_EQ(block.name(), "block");
		ASSERT_EQ(block.parentTypes().size(), 0u);

		// Predicates
		ASSERT_EQ(domain.predicates().size(), 5u);

		const auto &on = *domain.predicates()[0];

		ASSERT_EQ(on.name(), "on");
		ASSERT_EQ(on.arguments().size(), 2u);
		ASSERT_EQ(on.arguments()[0]->name(), "x");
		const auto *onArgument0Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[0]->type());
		ASSERT_EQ(onArgument0Type, &block);
		ASSERT_EQ(on.arguments()[1]->name(), "y");
		const auto onArgument1Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[1]->type());
		ASSERT_EQ(onArgument1Type, &block);

		const auto &handempty = *domain.predicates()[3];

		ASSERT_EQ(handempty.name(), "handempty");
		ASSERT_TRUE(handempty.arguments().empty());
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseStorageDomain)
{
	using namespace plasp::pddl;

	try
	{
		const auto description = plasp::pddl::Description::fromStream(m_storageDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		// Name
		ASSERT_EQ(domain.name(), "storage-propositional");

		// Requirements
		ASSERT_EQ(domain.requirements().size(), 1u);
		ASSERT_EQ(domain.requirements()[0].type(), Requirement::Type::Typing);

		// Types
		ASSERT_EQ(domain.types().size(), 10u);

		const auto &hoist = *domain.types()[0];
		const auto &surface = *domain.types()[1];
		const auto &area = *domain.types()[3];
		const auto &object = *domain.types()[4];
		const auto &storearea = *domain.types()[7];
		const auto &crate = *domain.types()[9];

		const auto &hoistParents = hoist.parentTypes();
		ASSERT_EQ(hoistParents.size(), 1u);
		ASSERT_TRUE(std::find(hoistParents.cbegin(), hoistParents.cend(), &object) != hoistParents.cend());

		const auto &areaParents = area.parentTypes();
		ASSERT_EQ(areaParents.size(), 2u);
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &object) != areaParents.cend());
		ASSERT_TRUE(std::find(areaParents.cbegin(), areaParents.cend(), &surface) != areaParents.cend());

		// Predicates
		ASSERT_EQ(domain.predicates().size(), 8u);

		const auto &on = *domain.predicates()[5];

		ASSERT_EQ(on.name(), "on");
		ASSERT_EQ(on.arguments().size(), 2u);
		ASSERT_EQ(on.arguments()[0]->name(), "c");
		const auto onArgument0Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[0]->type());
		ASSERT_EQ(onArgument0Type, &crate);
		ASSERT_EQ(on.arguments()[1]->name(), "s");
		const auto onArgument1Type = dynamic_cast<const expressions::PrimitiveType *>(on.arguments()[1]->type());
		ASSERT_EQ(onArgument1Type, &storearea);

		const auto &in = *domain.predicates()[1];
		ASSERT_EQ(in.name(), "in");
		ASSERT_EQ(in.arguments().size(), 2u);
		ASSERT_EQ(in.arguments()[0]->name(), "x");
		const auto inArgument0Type = dynamic_cast<const expressions::Either *>(in.arguments()[0]->type());
		ASSERT_EQ(inArgument0Type->arguments().size(), 2u);
		const auto inArgument0Type0 = dynamic_cast<const expressions::Reference<expressions::PrimitiveType> *>(inArgument0Type->arguments()[0].get())->value();
		ASSERT_EQ(inArgument0Type0, &storearea);
		const auto inArgument0Type1 = dynamic_cast<const expressions::Reference<expressions::PrimitiveType> *>(inArgument0Type->arguments()[1].get())->value();
		ASSERT_EQ(inArgument0Type1, &crate);
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseConstants)
{
	using namespace plasp::pddl;

	try
	{
		const auto description = Description::fromStream(m_woodworkingDomainFile);

		ASSERT_NO_THROW(description.domain());

		const auto &domain = description.domain();

		// Name
		ASSERT_EQ(domain.name(), "woodworking");

		// Types
		const auto &acolour = *domain.types()[0];
		const auto &surface = *domain.types()[4];
		const auto &treatmentstatus = *domain.types()[5];

		// Constants
		ASSERT_EQ(domain.constants().size(), 8u);
		ASSERT_EQ(domain.constants()[0]->type(), &surface);
		ASSERT_EQ(domain.constants()[2]->type(), &surface);
		ASSERT_EQ(domain.constants()[3]->type(), &treatmentstatus);
		ASSERT_EQ(domain.constants()[6]->type(), &treatmentstatus);
		ASSERT_EQ(domain.constants()[7]->type(), &acolour);
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_F(PDDLParserTests, ParseWithWhiteSpace)
{
	using namespace plasp::pddl;

	try
	{
		ASSERT_NO_THROW(Description::fromStream(m_whiteSpaceTestFile));
	}
	catch (const std::exception &e)
	{
		FAIL() << e.what();
	}
}
