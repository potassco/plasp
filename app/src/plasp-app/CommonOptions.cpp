#include <plasp-app/CommonOptions.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Common Options
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void addBasicOptions(cxxopts::Options &options)
{
	options.add_options("basic")
		("h,help", "Display this help message")
		("v,version", "Display version information")
		("warnings-as-errors", "Treat warnings as errors");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void addOutputOptions(cxxopts::Options &options)
{
	options.add_options("output")
		("color", "Colorize output (always, never, auto)", cxxopts::value<std::string>()->default_value("auto"))
		("p,log-priority", "Log messages starting from this priority (debug, info, warning, error)", cxxopts::value<std::string>()->default_value("info"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void addParserOptions(cxxopts::Options &options)
{
	options.add_options("parser")
		("i,input", "Input files (in PDDL or SAS format)", cxxopts::value<std::vector<std::string>>())
		("parsing-mode", "Parsing mode (strict, compatibility)", cxxopts::value<std::string>()->default_value("strict"))
		("l,language", "Input language (pddl, sas, auto)", cxxopts::value<std::string>()->default_value("auto"));
	options.parse_positional("input");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

BasicOptions parseBasicOptions(cxxopts::Options &options)
{
	BasicOptions basicOptions;

	basicOptions.help = options["help"].as<bool>();
	basicOptions.version = options["version"].as<bool>();
	basicOptions.warningsAsErrors = options["warnings-as-errors"].as<bool>();

	return basicOptions;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

OutputOptions parseOutputOptions(cxxopts::Options &options)
{
	OutputOptions outputOptions;

	const auto colorPolicy = options["color"].as<std::string>();

	if (colorPolicy == "auto")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Auto;
	else if (colorPolicy == "never")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Never;
	else if (colorPolicy == "always")
		outputOptions.colorPolicy = colorlog::ColorStream::ColorPolicy::Always;
	else
		throw OptionException("unknown color policy “" + colorPolicy + "”");

	const auto logPriorityString = options["log-priority"].as<std::string>();

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

ParserOptions parseParserOptions(cxxopts::Options &options)
{
	ParserOptions parserOptions;

	const auto parsingModeString = options["parsing-mode"].as<std::string>();

	if (parsingModeString == "compatibility")
		parserOptions.parsingMode = pddl::Mode::Compatibility;
	else if (parsingModeString != "strict")
		throw OptionException("unknown parsing mode “" + parsingModeString + "”");

	if (options.count("input"))
		parserOptions.inputFiles = options["input"].as<std::vector<std::string>>();

	const auto languageName = options["language"].as<std::string>();
	parserOptions.language = plasp::Language::fromString(languageName);

	if (parserOptions.language == plasp::Language::Type::Unknown)
		throw OptionException("unknown input language “" + languageName + "”");

	return parserOptions;
}
