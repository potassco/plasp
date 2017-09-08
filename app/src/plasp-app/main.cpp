#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

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

#include <plasp-app/CommonOptions.h>
#include <plasp-app/Version.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description;
	description.add(basicOptions());
	description.add(outputOptions());
	description.add(parserOptions());

	po::variables_map variablesMap;

	const auto printHelp =
		[&]()
		{
			std::cout << "Usage: plasp [options] file..." << std::endl;
			std::cout << "Translate PDDL to ASP." << std::endl;

			std::cout << description;
		};

	colorlog::Logger logger;

	try
	{
		po::store(po::command_line_parser(argc, argv)
			.options(description)
			.positional(parserPositionalOptions())
			.run(),
			variablesMap);
		po::notify(variablesMap);
	}
	catch (const po::error &e)
	{
		logger.log(colorlog::Priority::Error, e.what());
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	BasicOptions basicOptions;
	OutputOptions outputOptions;
	ParserOptions parserOptions;

	try
	{
		basicOptions = parseBasicOptions(variablesMap);

		if (basicOptions.help)
		{
			printHelp();
			return EXIT_SUCCESS;
		}

		if (basicOptions.version)
		{
			std::cout << Version << std::endl;
			return EXIT_SUCCESS;
		}

		if (basicOptions.warningsAsErrors)
			logger.setAbortPriority(colorlog::Priority::Warning);

		const auto outputOptions = parseOutputOptions(variablesMap);

		logger.setColorPolicy(outputOptions.colorPolicy);
		logger.setLogPriority(outputOptions.logPriority);

		parserOptions = parseParserOptions(variablesMap);
	}
	catch (const OptionException &e)
	{
		logger.log(colorlog::Priority::Error, e.what());
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	const auto printCompatibilityInfo =
		[&]()
		{
			if (parserOptions.parsingMode != pddl::Mode::Compatibility)
				logger.log(colorlog::Priority::Info, "try using --parsing-mode=compatibility for extended legacy feature support");
		};

	try
	{
		tokenize::Tokenizer<tokenize::CaseInsensitiveTokenizerPolicy> tokenizer;

		std::for_each(parserOptions.inputFiles.cbegin(), parserOptions.inputFiles.cend(),
			[&](const auto &inputFile)
			{
				tokenizer.read(inputFile);
			});

		if (parserOptions.inputFiles.empty())
			tokenizer.read("std::cin", std::cin);

		if (parserOptions.language == plasp::Language::Type::Automatic)
			parserOptions.language = plasp::detectLanguage(tokenizer);

		if (parserOptions.language == plasp::Language::Type::PDDL)
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
		}
		else if (parserOptions.language == plasp::Language::Type::SAS)
		{
			const auto description = plasp::sas::Description::fromTokenizer(std::move(tokenizer));
			const auto translator = plasp::sas::TranslatorASP(description, logger.outputStream());
			translator.translate();
		}
		else
			throw std::runtime_error("language detection failed");
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
