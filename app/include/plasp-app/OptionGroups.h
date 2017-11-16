#ifndef __PLASP_APP__OPTION_GROUPS_H
#define __PLASP_APP__OPTION_GROUPS_H

#include <cxxopts.hpp>

#include <colorlog/ColorStream.h>
#include <colorlog/Priority.h>

#include <pddl/Exception.h>
#include <pddl/Mode.h>

#include <plasp/Language.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Option Groups
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class OptionException : public pddl::Exception
{
	public:
		using Exception::Exception;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct OptionGroupBasic
{
	static constexpr const auto Name = "basic";

	void addTo(cxxopts::Options &options);
	void read(const cxxopts::ParseResult &parseResult);

	bool help = false;
	bool version = false;
	bool warningsAsErrors = false;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct OptionGroupOutput
{
	static constexpr const auto Name = "output";

	void addTo(cxxopts::Options &options);
	void read(const cxxopts::ParseResult &parseResult);

	colorlog::ColorStream::ColorPolicy colorPolicy = colorlog::ColorStream::ColorPolicy::Auto;
	colorlog::Priority logPriority = colorlog::Priority::Info;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct OptionGroupParser
{
	static constexpr const auto Name = "parser";

	void addTo(cxxopts::Options &options);
	void read(const cxxopts::ParseResult &parseResult);

	std::vector<std::string> inputFiles;
	pddl::Mode parsingMode = pddl::Mode::Strict;
	plasp::Language::Type language = plasp::Language::Type::Automatic;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
