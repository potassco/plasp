#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <plasp/LanguageDetection.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/TranslatorASP.h>
#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>
#include <plasp/utils/TranslatorException.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "Display this help message.")
		("version,v", "Display version information.")
		("input,i", po::value<std::vector<std::string>>(), "Specify the SAS input file.")
		("language,l", po::value<std::string>(), "Specify the input language (SAS or PDDL). Omit for automatic detection.");

	po::positional_options_description positionalOptionsDescription;
	positionalOptionsDescription.add("input", -1);

	po::variables_map variablesMap;

	const auto printHelp =
		[&]()
		{
			std::cout << "Usage: plasp file [options]" << std::endl;
			std::cout << "Translate PDDL instances to ASP facts." << std::endl << std::endl;

			std::cout << description;
		};

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
		std::cerr << "Error: " << e.what() << std::endl << std::endl;
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
		std::cout << "plasp version 3.0.0" << std::endl;
		return EXIT_SUCCESS;
	}

	try
	{
		plasp::utils::Parser parser;

		parser.setCaseSensitive(false);

		if (variablesMap.count("input"))
		{
			const auto &inputFiles = variablesMap["input"].as<std::vector<std::string>>();

			std::for_each(inputFiles.cbegin(), inputFiles.cend(),
				[&](const auto &inputFile)
				{
					parser.readFile(inputFile);
				});
		}
		else
			parser.readStream("std::cin", std::cin);

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
			std::cerr << "Error: Unknown input language" << std::endl << std::endl;
			printHelp();
			return EXIT_FAILURE;
		}

		if (language == plasp::Language::Type::PDDL)
		{
			const auto description = plasp::pddl::Description::fromParser(std::move(parser));
			const auto translator = plasp::pddl::TranslatorASP(description, std::cout);
			translator.translate();
		}
		else if (language == plasp::Language::Type::SAS)
		{
			const auto description = plasp::sas::Description::fromParser(std::move(parser));
			const auto translator = plasp::sas::TranslatorASP(description);
			translator.translate(std::cout);
		}
	}
	catch (const plasp::utils::ParserException &e)
	{
		std::cerr << "Parser error: " << e.what() << std::endl << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}
	catch (const plasp::utils::TranslatorException &e)
	{
		std::cerr << "Translation error: " << e.what() << std::endl << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
