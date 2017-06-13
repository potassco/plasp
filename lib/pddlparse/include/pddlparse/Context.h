#ifndef __PDDL_PARSE__CONTEXT_H
#define __PDDL_PARSE__CONTEXT_H

#include <functional>

#include <pddlparse/Tokenizer.h>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Context
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Context
{
	constexpr static const char *auxiliaryPrefix()
	{
		return "__plasp_";
	}

	// TODO: replace std::string with char *
	using WarningCallback = std::function<void (tokenize::Location, const std::string &)>;

	Context() = default;
	~Context() = default;

	explicit Context(Tokenizer &&tokenizer, WarningCallback warningCallback)
	:	tokenizer{std::move(tokenizer)},
		warningCallback{warningCallback}
	{
	}

	Context(const Context &other) = delete;
	Context &operator=(const Context &other) = delete;
	Context(Context &&other) = default;
	Context &operator=(Context &&other) = default;

	Tokenizer tokenizer;
	WarningCallback warningCallback;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
