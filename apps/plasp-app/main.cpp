#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <plasp/LanguageDetection.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/TranslatorASP.h>
#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>
#include <plasp/utils/LogStream.h>
#include <plasp/utils/TranslatorException.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "Display this help message.")
		("version,v", "Display version information.")
		("input,i", po::value<std::vector<std::string>>(), "Specify the PDDL or SAS input file.")
		("language,l", po::value<std::string>(), "Specify the input language (sas or pddl). Omit for automatic detection.")
		("warning-level", po::value<std::string>()->default_value("normal"), "Specify whether to output warnings normally (normal), to treat them as critical errors (error), or to ignore them (ignore).")
		("color", po::value<std::string>()->default_value("auto"), "Specify whether to colorize the output (always, never, or auto).");

	po::positional_options_description positionalOptionsDescription;
	positionalOptionsDescription.add("input", -1);

	po::variables_map variablesMap;

	const auto printHelp =
		[&]()
		{
			std::cout << "Usage: plasp [files] [options]" << std::endl;
			std::cout << "Translate PDDL instances to ASP facts." << std::endl << std::endl;

			std::cout << description;
		};

	plasp::utils::Logger logger;

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
		logger.logError(e.what());
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
		std::cout << "plasp version 3.0.2-rc.1-git" << std::endl;
		return EXIT_SUCCESS;
	}

	const auto warningLevel = variablesMap["warning-level"].as<std::string>();

	if (warningLevel == "error")
		logger.setWarningLevel(plasp::utils::Logger::WarningLevel::Error);
	else if (warningLevel == "ignore")
		logger.setWarningLevel(plasp::utils::Logger::WarningLevel::Ignore);
	else if (warningLevel == "normal")
		logger.setWarningLevel(plasp::utils::Logger::WarningLevel::Normal);
	else
	{
		logger.logError("unknown warning level “" + warningLevel + "”");
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	const auto colorPolicy = variablesMap["color"].as<std::string>();

	if (colorPolicy == "auto")
		logger.setColorPolicy(plasp::utils::LogStream::ColorPolicy::Auto);
	else if (colorPolicy == "never")
		logger.setColorPolicy(plasp::utils::LogStream::ColorPolicy::Never);
	else if (colorPolicy == "always")
		logger.setColorPolicy(plasp::utils::LogStream::ColorPolicy::Always);
	else
	{
		logger.logError("unknown color policy “" + colorPolicy + "”");
		std::cout << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	try
	{
		plasp::utils::Parser<plasp::utils::CaseInsensitiveParserPolicy> parser;

		if (variablesMap.count("input"))
		{
			const auto &inputFiles = variablesMap["input"].as<std::vector<std::string>>();

			std::for_each(inputFiles.cbegin(), inputFiles.cend(),
				[&](const auto &inputFile)
				{
					parser.read(inputFile);
				});
		}
		else
			parser.read("std::cin", std::cin);

		const auto detectLanguage =
			[&]()
			{
				if (variablesMap.count("language") == 0)
					return plasp::detectLanguage(parser);

				const auto languageName = variablesMap["language"].as<std::string>();

				return plasp::Language::fromString(languageName);
			};

		const auto language = detectLanguage();

		if (language == plasp::Language::Type::Unknown)
		{
			logger.logError("unknown input language");
			std::cout << std::endl;
			printHelp();
			return EXIT_FAILURE;
		}

		if (language == plasp::Language::Type::PDDL)
		{
			auto pddlLogger = logger;
			auto context = plasp::pddl::Context(std::move(parser), std::move(pddlLogger));
			auto description = plasp::pddl::Description::fromContext(std::move(context));
			const auto translator = plasp::pddl::TranslatorASP(description, description.context().logger.outputStream());
			translator.translate();
		}
		else if (language == plasp::Language::Type::SAS)
		{
			const auto description = plasp::sas::Description::fromParser(std::move(parser));
			const auto translator = plasp::sas::TranslatorASP(description, logger.outputStream());
			translator.translate();
		}
	}
	catch (const plasp::utils::ParserException &e)
	{
		logger.logError(e.coordinate(), e.message());
		return EXIT_FAILURE;
	}
	catch (const plasp::utils::TranslatorException &e)
	{
		logger.logError(e.what());
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		logger.logError(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
