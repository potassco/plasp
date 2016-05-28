#include <iostream>

#include <boost/program_options.hpp>

#include <plasp/sas/Description.h>
#include <plasp/sas/TranslatorASP.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "Display this help message.")
		("version,v", "Display version information.")
		("input,i", po::value<std::string>(), "Specify the SAS input file.");

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
		const auto sasDescription = variablesMap.count("input")
			? plasp::sas::Description::fromFile(variablesMap["input"].as<std::string>())
			: plasp::sas::Description::fromStream(std::cin);
		const auto sasTranslator = plasp::sas::TranslatorASP(sasDescription);
		sasTranslator.translate(std::cout);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl << std::endl;
		printHelp();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
