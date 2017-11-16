#include <plasp-app/OptionGroups.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Option Groups
//
////////////////////////////////////////////////////////////////////////////////////////////////////

// Nasty workaround needed for GCC prior to version 7
constexpr decltype(OptionGroupBasic::Name) OptionGroupBasic::Name;

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupBasic::addTo(cxxopts::Options &options)
{
	options.add_options(Name)
		("h,help", "Display this help message")
		("v,version", "Display version information")
		("warnings-as-errors", "Treat warnings as errors");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupBasic::read(const cxxopts::ParseResult &parseResult)
{
	help = (parseResult.count("help") > 0);
	version = (parseResult.count("version") > 0);
	warningsAsErrors = (parseResult.count("warnings-as-errors") > 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Nasty workaround needed for GCC prior to version 7
constexpr decltype(OptionGroupOutput::Name) OptionGroupOutput::Name;

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupOutput::addTo(cxxopts::Options &options)
{
	options.add_options(Name)
		("color", "Colorize output (always, never, auto)", cxxopts::value<std::string>()->default_value("auto"))
		("p,log-priority", "Log messages starting from this priority (debug, info, warning, error)", cxxopts::value<std::string>()->default_value("info"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupOutput::read(const cxxopts::ParseResult &parseResult)
{
	const auto colorPolicyString = parseResult["color"].as<std::string>();

	if (colorPolicyString == "auto")
		colorPolicy = colorlog::ColorStream::ColorPolicy::Auto;
	else if (colorPolicyString == "never")
		colorPolicy = colorlog::ColorStream::ColorPolicy::Never;
	else if (colorPolicyString == "always")
		colorPolicy = colorlog::ColorStream::ColorPolicy::Always;
	else
		throw OptionException("unknown color policy “" + colorPolicyString + "”");

	const auto logPriorityString = parseResult["log-priority"].as<std::string>();

	try
	{
		logPriority = colorlog::priorityFromName(logPriorityString.c_str());
	}
	catch (const std::exception &e)
	{
		throw OptionException(e.what());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// Nasty workaround needed for GCC prior to version 7
constexpr decltype(OptionGroupParser::Name) OptionGroupParser::Name;

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupParser::addTo(cxxopts::Options &options)
{
	options.add_options(Name)
		("i,input", "Input files (in PDDL or SAS format)", cxxopts::value<std::vector<std::string>>())
		("parsing-mode", "Parsing mode (strict, compatibility)", cxxopts::value<std::string>()->default_value("strict"))
		("l,language", "Input language (pddl, sas, auto)", cxxopts::value<std::string>()->default_value("auto"));
	options.parse_positional("input");
	options.positional_help("[<input file...>]");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void OptionGroupParser::read(const cxxopts::ParseResult &parseResult)
{
	const auto parsingModeString = parseResult["parsing-mode"].as<std::string>();

	if (parsingModeString == "compatibility")
		parsingMode = pddl::Mode::Compatibility;
	else if (parsingModeString != "strict")
		throw OptionException("unknown parsing mode “" + parsingModeString + "”");

	if (parseResult.count("input"))
		inputFiles = parseResult["input"].as<std::vector<std::string>>();

	const auto languageName = parseResult["language"].as<std::string>();
	language = plasp::Language::fromString(languageName);

	if (language == plasp::Language::Type::Unknown)
		throw OptionException("unknown input language “" + languageName + "”");
}
