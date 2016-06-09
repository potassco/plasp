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

Language::Type detectLanguage(utils::Parser &parser)
{
	// PDDL contains sections starting with "(define"
	if (parser.probe<std::string>("(") && parser.probe<std::string>("define"))
	{
		parser.seek(std::ios::beg);
		return Language::Type::PDDL;
	}

	// SAS begins with "begin_version"
	if (parser.probe<std::string>("begin"))
	{
		parser.seek(std::ios::beg);
		return Language::Type::SAS;
	}

	parser.seek(std::ios::beg);
	return Language::Type::Unknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
