#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <plasp/pddl/Description.h>
#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "Display this help message.")
		("version,v", "Display version information.")
		("input,i", po::value<std::vector<std::string>>(), "Specify the SAS input file.")
		("format,f", po::value<std::string>()->default_value("SAS"), "Specify the file format (SAS or PDDL).");

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

	auto format = variablesMap["format"].as<std::string>();
	std::transform(format.begin(), format.end(), format.begin(), ::tolower);

	if (format == "sas")
	{
		if (variablesMap.count("input"))
		{
			const auto &inputFiles = variablesMap["input"].as<std::vector<std::string>>();

			if (inputFiles.size() > 1)
			{
				std::cerr << "Error: Only one input file allowed for SAS translation" << std::endl;
				printHelp();
				return EXIT_FAILURE;
			}

			const auto sasDescription = plasp::sas::Description::fromFile(inputFiles.front());
			const auto sasTranslator = plasp::sas::TranslatorASP(sasDescription);
			sasTranslator.translate(std::cout);
		}
		else
		{
			const auto sasDescription = plasp::sas::Description::fromStream(std::cin);
			const auto sasTranslator = plasp::sas::TranslatorASP(sasDescription);
			sasTranslator.translate(std::cout);
		}
	}
	else if (format == "pddl")
	{
		if (variablesMap.count("input"))
		{
			const auto &inputFiles = variablesMap["input"].as<std::vector<std::string>>();
			const auto pddlDescription = plasp::pddl::Description::fromFiles(inputFiles);
		}
		else
			const auto pddlDescription = plasp::pddl::Description::fromStream(std::cin);

		//std::cout << pddlDescription << std::endl;
	}

	return EXIT_SUCCESS;
}
