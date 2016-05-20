#include <iostream>

#include <boost/program_options.hpp>

#include <plasp/sas/Description.h>

int main(int argc, char **argv)
{
	namespace po = boost::program_options;

	po::options_description description("Allowed options");
	description.add_options()
		("help,h", "display this help message")
		("input,i", po::value<std::string>(), "SAS input file");

	po::positional_options_description positionalOptionsDescription;
	positionalOptionsDescription.add("input", -1);

	po::variables_map variablesMap;
	po::store(po::command_line_parser(argc, argv)
		.options(description)
		.positional(positionalOptionsDescription)
		.run(),
		variablesMap);
	po::notify(variablesMap);

	if (variablesMap.count("help"))
	{
		std::cout << description;
		return EXIT_SUCCESS;
	}

	if (!variablesMap.count("input"))
	{
		std::cerr << "Error: No input file specified" << std::endl;
		std::cout << description;
		return EXIT_FAILURE;
	}

	const auto sasDescription = plasp::sas::Description::fromFile(variablesMap["input"].as<std::string>());
	sasDescription.print(std::cout);

	return EXIT_SUCCESS;
}
