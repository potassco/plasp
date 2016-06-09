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

	auto format = variablesMap["format"].as<std::string>();
	std::transform(format.begin(), format.end(), format.begin(), ::tolower);

	if (format == "sas")
	{
		const auto sasDescription = plasp::sas::Description::fromParser(std::move(parser));
		const auto sasTranslator = plasp::sas::TranslatorASP(sasDescription);
		sasTranslator.translate(std::cout);
	}
	else if (format == "pddl")
		plasp::pddl::Description::fromParser(std::move(parser));

	return EXIT_SUCCESS;
}
