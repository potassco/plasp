#ifndef __PLASP__LANGUAGE_DETECTION_H
#define __PLASP__LANGUAGE_DETECTION_H

#include <tokenize/Tokenizer.h>

#include <plasp/Language.h>

namespace plasp
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// LanguageDetection
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline Language::Type detectLanguage(tokenize::Tokenizer<tokenize::CaseInsensitiveTokenizerPolicy> &tokenizer)
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

	// PDDL contains sections starting with “(define”
	// Some legacy domains contain “in-package” sections, however
	if (tokenizer.testAndSkip<std::string>("(")
		&& (tokenizer.testAndSkip<std::string>("define") || tokenizer.testAndSkip<std::string>("in-package")))
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
