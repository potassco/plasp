#ifndef __PLASP__LANGUAGE_DETECTION_H
#define __PLASP__LANGUAGE_DETECTION_H

#include <plasp/Language.h>
#include <plasp/utils/Parser.h>

namespace plasp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LanguageDetection
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Language::Type detectLanguage(utils::Parser<utils::CaseInsensitiveParserPolicy> &parser)
{
	parser.skipWhiteSpace();

	// SAS begins with "begin_version"
	if (parser.testAndSkip<std::string>("begin"))
	{
		parser.seek(0);
		return Language::Type::SAS;
	}

	// Skip potential PDDL comments
	while (parser.currentCharacter() == ';')
	{
		parser.skipLine();
		parser.skipWhiteSpace();
	}

	// PDDL contains sections starting with "(define"
	if (parser.testAndSkip<std::string>("(") && parser.testAndSkip<std::string>("define"))
	{
		parser.seek(std::ios::beg);
		return Language::Type::PDDL;
	}

	parser.seek(std::ios::beg);
	return Language::Type::Unknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
