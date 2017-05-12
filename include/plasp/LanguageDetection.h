#ifndef __PLASP__LANGUAGE_DETECTION_H
#define __PLASP__LANGUAGE_DETECTION_H

#include <plasp/Language.h>

#include <tokenize/Tokenizer.h>

namespace plasp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LanguageDetection
//
////////////////////////////////////////////////////////////////////////////////////////////////////

Language::Type detectLanguage(tokenize::Tokenizer<tokenize::CaseInsensitiveTokenizerPolicy> &tokenizer)
{
	tokenizer.skipWhiteSpace();

	// SAS begins with "begin_version"
	if (tokenizer.testAndSkip<std::string>("begin"))
	{
		tokenizer.seek(0);
		return Language::Type::SAS;
	}

	// Skip potential PDDL comments
	while (tokenizer.currentCharacter() == ';')
	{
		tokenizer.skipLine();
		tokenizer.skipWhiteSpace();
	}

	// PDDL contains sections starting with "(define"
	if (tokenizer.testAndSkip<std::string>("(") && tokenizer.testAndSkip<std::string>("define"))
	{
		tokenizer.seek(std::ios::beg);
		return Language::Type::PDDL;
	}

	tokenizer.seek(std::ios::beg);
	return Language::Type::Unknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
