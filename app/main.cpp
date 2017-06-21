#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <pddlparse/AST.h>
#include <pddlparse/Exception.h>
#include <pddlparse/Mode.h>
#include <pddlparse/Parse.h>

#include <plasp/LanguageDetection.h>
#include <plasp/output/ColorStream.h>
#include <plasp/output/Logger.h>
#include <plasp/output/Priority.h>
#include <plasp/output/TranslatorException.h>
#include <plasp/pddl/TranslatorASP.h>
#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	bool warningsAsErrors;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "Display this help message")
		("version,v", "Display version information")
		("input,i", po::value<std::vector<std::string>>(), "Input files (in PDDL or SAS format)")
		("parsing-mode", po::value<std::string>()->default_value("strict"), "Parsing mode (strict, compatibility)")
		("language,l", po::value<std::string>()->default_value("auto"), "Input language (pddl, sas, auto)")
		("color", po::value<std::string>()->default_value("auto"), "Colorize output (always, never, auto)")
		("log-priority,p", po::value<std::string>()->default_value("info"), "Log messages starting from this priority (debug, info, warning, error)")
		("warnings-as-errors", po::bool_switch(&warningsAsErrors), "Treat warnings as errors");

	po::positional_options_description positionalOptionsDescription;
	positionalOptionsDescription.add("input", -1);

	po::variables_map variablesMap;

	const auto printHelp =
		[&]()
		{
			std::cout << "Usage: plasp [options] file..." << std::endl;
			std::cout << "Translate PDDL to ASP." << std::endl << std::endl;

			std::cout << description;
		};

	plasp::output::Logger logger;

	try
	{
		po::store(po::command_line_parser(argc, argv)
			.options(description)
			.positional(positionalOptionsDescription)
			.run(),
			variablesMap);
		po::notify(variablesMap);
	}
	catch (const po::error &e)
	{
		logger.log(plasp::output::Priority::Error, e.what());
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	if (variablesMap.count("help"))
	{
		printHelp();
		return EXIT_SUCCESS;
	}

	if (variablesMap.count("version"))
	{
		std::cout << "plasp version 3.0.3-git" << std::endl;
		return EXIT_SUCCESS;
	}

	if (warningsAsErrors)
		logger.setAbortPriority(plasp::output::Priority::Warning);

	auto parsingMode = pddl::Mode::Strict;

	const auto parsingModeString = variablesMap["parsing-mode"].as<std::string>();

	if (parsingModeString == "compatibility")
		parsingMode = pddl::Mode::Compatibility;
	else if (parsingModeString != "strict")
	{
		logger.log(plasp::output::Priority::Error, "unknown parsing mode “" + parsingModeString + "”");
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	const auto printCompatibilityInfo =
		[&]()
		{
			if (parsingMode != pddl::Mode::Compatibility)
				logger.log(plasp::output::Priority::Info, "try using --parsing-mode=compatibility for extended legacy feature support");
		};

	const auto colorPolicy = variablesMap["color"].as<std::string>();

	if (colorPolicy == "auto")
		logger.setColorPolicy(plasp::output::ColorStream::ColorPolicy::Auto);
	else if (colorPolicy == "never")
		logger.setColorPolicy(plasp::output::ColorStream::ColorPolicy::Never);
	else if (colorPolicy == "always")
		logger.setColorPolicy(plasp::output::ColorStream::ColorPolicy::Always);
	else
	{
		logger.log(plasp::output::Priority::Error, "unknown color policy “" + colorPolicy + "”");
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	const auto logPriorityString = variablesMap["log-priority"].as<std::string>();

	try
	{
		const auto logPriority = plasp::output::priorityFromName(logPriorityString.c_str());
		logger.setLogPriority(logPriority);
	}
	catch (const std::exception &e)
	{
		logger.log(plasp::output::Priority::Error, ("unknown log priorty “" + logPriorityString + "”").c_str());
		logger.errorStream() << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	try
	{
		tokenize::Tokenizer<tokenize::CaseInsensitiveTokenizerPolicy> tokenizer;

		if (variablesMap.count("input"))
		{
			const auto &inputFiles = variablesMap["input"].as<std::vector<std::string>>();

			std::for_each(inputFiles.cbegin(), inputFiles.cend(),
				[&](const auto &inputFile)
				{
					tokenizer.read(inputFile);
				});
		}
		else
			tokenizer.read("std::cin", std::cin);

		const auto detectLanguage =
			[&]()
			{
				const auto languageName = variablesMap["language"].as<std::string>();
				const auto language = plasp::Language::fromString(languageName);

				if (language == plasp::Language::Type::Automatic)
					return plasp::detectLanguage(tokenizer);

				return language;
			};

		const auto language = detectLanguage();

		if (language == plasp::Language::Type::Unknown)
		{
			logger.log(plasp::output::Priority::Error, "unknown input language");
			std::cout << std::endl;
			printHelp();
			return EXIT_FAILURE;
		}

		if (language == plasp::Language::Type::PDDL)
		{
			const auto logWarning =
				[&](const auto &location, const auto &warning)
				{
					logger.log(plasp::output::Priority::Warning, location, warning);
				};

			auto context = pddl::Context(std::move(tokenizer), logWarning);
			context.mode = parsingMode;
			auto description = pddl::parseDescription(context);
			const auto translator = plasp::pddl::TranslatorASP(std::move(description), logger.outputStream());
			translator.translate();
		}
		else if (language == plasp::Language::Type::SAS)
		{
			const auto description = plasp::sas::Description::fromTokenizer(std::move(tokenizer));
			const auto translator = plasp::sas::TranslatorASP(description, logger.outputStream());
			translator.translate();
		}
	}
	catch (const tokenize::TokenizerException &e)
	{
		logger.log(plasp::output::Priority::Error, e.location(), e.message().c_str());

		printCompatibilityInfo();

		return EXIT_FAILURE;
	}
	catch (const pddl::ParserException &e)
	{
		if (e.location())
			logger.log(plasp::output::Priority::Error, e.location().value(), e.message().c_str());
		else
			logger.log(plasp::output::Priority::Error, e.message().c_str());

		printCompatibilityInfo();

		return EXIT_FAILURE;
	}
	catch (const plasp::output::TranslatorException &e)
	{
		logger.log(plasp::output::Priority::Error, e.what());
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		logger.log(plasp::output::Priority::Error, e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
