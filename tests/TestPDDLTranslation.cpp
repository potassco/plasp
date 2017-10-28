#include <catch.hpp>

#include <iostream>

#include <colorlog/Logger.h>

#include <pddl/AST.h>
#include <pddl/Normalize.h>
#include <pddl/Parse.h>

#include <plasp/pddl/TranslatorASP.h>

#include "NullOutputStream.h"

const pddl::Context::WarningCallback ignoreWarnings = [](const auto &, const auto &){};

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("[PDDL translation] Former issues are fixed", "[PDDL translation]")
{
	NullOutputStream nullOutputStream;

	colorlog::Logger logger(nullOutputStream, nullOutputStream);
	pddl::Tokenizer tokenizer;
	pddl::Context context(std::move(tokenizer), ignoreWarnings);

	SECTION("translating domains without typing information works")
	{
		context.tokenizer.read("data/issues/issue-4.pddl");
		auto description = pddl::normalize(pddl::parseDescription(context));
		const auto translator = plasp::pddl::TranslatorASP(std::move(description), logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}

	SECTION("translating the simple blocks world domain works")
	{
		context.tokenizer.read("data/issues/issue-5.pddl");
		auto description = pddl::normalize(pddl::parseDescription(context));
		const auto translator = plasp::pddl::TranslatorASP(std::move(description), logger.outputStream());
		CHECK_NOTHROW(translator.translate());
	}
}
