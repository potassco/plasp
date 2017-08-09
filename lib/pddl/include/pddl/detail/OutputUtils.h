#ifndef __PDDL__DETAIL__OUTPUT_UTILS_H
#define __PDDL__DETAIL__OUTPUT_UTILS_H

#include <colorlog/Formatting.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// OutputUtils
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct PrintContext
{
	size_t indentationLevel{0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Identifier
{
	Identifier(const char *content)
	:	content{content}
	{
	}

	Identifier(const std::string &content)
	:	content{content.c_str()}
	{
	}

	const char *content;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Identifier &identifier)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Green, colorlog::FontWeight::Normal})
		<< identifier.content << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Type
{
	Type(const char *name)
	:	name{name}
	{
	}

	Type(const std::string &name)
	:	name{name.c_str()}
	{
	}

	const char *name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Type &type)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Red, colorlog::FontWeight::Normal})
		<< type.name << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Constant
{
	Constant(const char *name)
	:	name{name}
	{
	}

	Constant(const std::string &name)
	:	name{name.c_str()}
	{
	}

	const char *name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &operator<<(colorlog::ColorStream &stream, const Constant &constant)
{
	return (stream
		<< colorlog::Format({colorlog::Color::Yellow, colorlog::FontWeight::Normal})
		<< constant.name << colorlog::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline colorlog::ColorStream &printIndentedNewline(colorlog::ColorStream &stream, detail::PrintContext &printContext)
{
	stream << std::endl;

	for (size_t i = 0; i < printContext.indentationLevel; i++)
		stream << "\t";

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
