#include <plasp-app/commands/CommandTranslate.h>

#include <iostream>
#include <string>

#include <cxxopts.hpp>

#include <colorlog/ColorStream.h>
#include <colorlog/Logger.h>
#include <colorlog/Priority.h>

#include <pddl/AST.h>
#include <pddl/Exception.h>
#include <pddl/Mode.h>
#include <pddl/Normalize.h>
#include <pddl/NormalizedASTOutput.h>
#include <pddl/Parse.h>
#include <pddl/detail/normalization/Reduction.h>

#include <plasp/LanguageDetection.h>
#include <plasp/TranslatorException.h>

#include <plasp/pddl/TranslatorASP.h>
#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command Translate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

int CommandTranslate::run(int argc, char **argv)
{
	parseOptions(argc, argv);

	const auto &basicOptions = std::get<OptionGroupBasic>(m_optionGroups);
	const auto &outputOptions = std::get<OptionGroupOutput>(m_optionGroups);
	const auto &parserOptions = std::get<OptionGroupParser>(m_optionGroups);

	if (basicOptions.help)
	{
		printHelp();
		return EXIT_SUCCESS;
	}

	if (basicOptions.version)
	{
		printVersion();
		return EXIT_SUCCESS;
	}

	colorlog::Logger logger;
	logger.setColorPolicy(outputOptions.colorPolicy);
	logger.setLogPriority(outputOptions.logPriority);

	if (basicOptions.warningsAsErrors)
		logger.setAbortPriority(colorlog::Priority::Warning);

	const auto printCompatibilityInfo =
		[&]()
		{
			if (parserOptions.parsingMode != pddl::Mode::Compatibility)
				logger.log(colorlog::Priority::Info, "try using --parsing-mode=compatibility for extended legacy feature support");
		};

	try
	{
		tokenize::Tokenizer<tokenize::CaseInsensitiveTokenizerPolicy> tokenizer;

		if (!parserOptions.inputFiles.empty())
			std::for_each(parserOptions.inputFiles.cbegin(), parserOptions.inputFiles.cend(),
				[&](const auto &inputFile)
				{
					tokenizer.read(inputFile);
				});
		else
		{
			logger.log(colorlog::Priority::Info, "reading from stdin");
			tokenizer.read("std::cin", std::cin);
		}

		const auto detectLanguage =
			[&]()
			{
				if (parserOptions.language == plasp::Language::Type::Automatic)
					return plasp::detectLanguage(tokenizer);

				return parserOptions.language;
			};

		switch (detectLanguage())
		{
			case plasp::Language::Type::Automatic:
			case plasp::Language::Type::Unknown:
			{
				logger.log(colorlog::Priority::Error, "unknown input language");
				std::cout << std::endl;
				printHelp();
				return EXIT_FAILURE;
			}

			// TODO: get rid of unknown language type, use exception instead
			case plasp::Language::Type::PDDL:
			{
				const auto logWarning =
					[&](const auto &location, const auto &warning)
					{
						logger.log(colorlog::Priority::Warning, location, warning);
					};

				auto context = pddl::Context(std::move(tokenizer), logWarning);
				context.mode = parserOptions.parsingMode;
				auto description = pddl::parseDescription(context);
				auto normalizedDescription = pddl::normalize(std::move(description));
				const auto translator = plasp::pddl::TranslatorASP(std::move(normalizedDescription), logger.outputStream());
				translator.translate();
				return EXIT_SUCCESS;
			}

			case plasp::Language::Type::SAS:
			{
				const auto description = plasp::sas::Description::fromTokenizer(std::move(tokenizer));
				const auto translator = plasp::sas::TranslatorASP(description, logger.outputStream());
				translator.translate();
				return EXIT_SUCCESS;
			}
		}
	}
	catch (const tokenize::TokenizerException &e)
	{
		logger.log(colorlog::Priority::Error, e.location(), e.message().c_str());

		printCompatibilityInfo();

		return EXIT_FAILURE;
	}
	catch (const pddl::ParserException &e)
	{
		if (e.location())
			logger.log(colorlog::Priority::Error, e.location().value(), e.message().c_str());
		else
			logger.log(colorlog::Priority::Error, e.message().c_str());

		printCompatibilityInfo();

		return EXIT_FAILURE;
	}
	catch (const plasp::TranslatorException &e)
	{
		logger.log(colorlog::Priority::Error, e.what());
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		logger.log(colorlog::Priority::Error, e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
