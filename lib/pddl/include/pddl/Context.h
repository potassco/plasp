#ifndef __PDDL__CONTEXT_H
#define __PDDL__CONTEXT_H

#include <functional>

#include <pddl/Mode.h>
#include <pddl/Tokenizer.h>

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
	using WarningCallback = std::function<void (tokenize::Location &&, const std::string &)>;

	Context() = default;
	~Context() = default;

	explicit Context(Tokenizer &&tokenizer, WarningCallback warningCallback, Mode mode = Mode::Strict)
	:	tokenizer{std::move(tokenizer)},
		warningCallback{warningCallback},
		mode{mode}
	{
	}

	Context(const Context &other) = delete;
	Context &operator=(const Context &other) = delete;
	Context(Context &&other) = default;
	Context &operator=(Context &&other) = default;

	Tokenizer tokenizer;
	WarningCallback warningCallback;

	Mode mode;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
