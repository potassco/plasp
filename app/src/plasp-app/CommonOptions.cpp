#include <plasp-app/CommonOptions.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Common Options
//
////////////////////////////////////////////////////////////////////////////////////////////////////

po::options_description basicOptions()
{
	po::options_description basicOptions("Basic options");
	basicOptions.add_options()
		("help,h", po::bool_switch(), "Display this help message")
		("version,v", po::bool_switch(), "Display version information")
		("warnings-as-errors", po::bool_switch(), "Treat warnings as errors");

	return basicOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

po::options_description outputOptions()
{
	po::options_description outputOptions("Output options");
	outputOptions.add_options()
		("color", po::value<std::string>()->default_value("auto"), "Colorize output (always, never, auto)")
		("log-priority,p", po::value<std::string>()->default_value("info"), "Log messages starting from this priority (debug, info, warning, error)");

	return outputOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

po::options_description parserOptions()
{
	po::options_description parserOptions("Parser options options");
	parserOptions.add_options()
		("input,i", po::value<std::vector<std::string>>(), "Input files (in PDDL or SAS format)")
		("parsing-mode", po::value<std::string>()->default_value("strict"), "Parsing mode (strict, compatibility)")
		("language,l", po::value<std::string>()->default_value("auto"), "Input language (pddl, sas, auto)");

	return parserOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

po::positional_options_description parserPositionalOptions()
{
	po::positional_options_description positionalOptions;
	positionalOptions.add("input", -1);

	return positionalOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

BasicOptions parseBasicOptions(const po::variables_map &variablesMap)
{
	BasicOptions basicOptions;

	basicOptions.help = variablesMap["help"].as<bool>();
	basicOptions.version = variablesMap["version"].as<bool>();
	basicOptions.warningsAsErrors = variablesMap["warnings-as-errors"].as<bool>();

	return basicOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

OutputOptions parseOutputOptions(const po::variables_map &variablesMap)
{
	OutputOptions outputOptions;

	const auto colorPolicy = variablesMap["color"].as<std::string>();

	if (colorPolicy == "auto")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Auto;
	else if (colorPolicy == "never")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Never;
	else if (colorPolicy == "always")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Always;
	else
		throw OptionException("unknown color policy “" + colorPolicy + "”");

	const auto logPriorityString = variablesMap["log-priority"].as<std::string>();

	try
	{
		outputOptions.logPriority = colorlog::priorityFromName(logPriorityString.c_str());
	}
	catch (const std::exception &e)
	{
		throw OptionException(e.what());
	}

	return outputOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ParserOptions parseParserOptions(const po::variables_map &variablesMap)
{
	ParserOptions parserOptions;

	const auto parsingModeString = variablesMap["parsing-mode"].as<std::string>();

	if (parsingModeString == "compatibility")
		parserOptions.parsingMode = pddl::Mode::Compatibility;
	else if (parsingModeString != "strict")
		throw OptionException("unknown parsing mode “" + parsingModeString + "”");

	if (variablesMap.count("input"))
		parserOptions.inputFiles = variablesMap["input"].as<std::vector<std::string>>();

	const auto languageName = variablesMap["language"].as<std::string>();
	parserOptions.language = plasp::Language::fromString(languageName);

	if (parserOptions.language == plasp::Language::Type::Unknown)
		throw OptionException("unknown input language “" + languageName + "”");

	return parserOptions;
}
