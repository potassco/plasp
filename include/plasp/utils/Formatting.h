#ifndef __PLASP__UTILS__FORMATTING_H
#define __PLASP__UTILS__FORMATTING_H

#include <iostream>

#include <plasp/utils/LogStream.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Formatting
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class Color
{
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7
};

////////////////////////////////////////////////////////////////////////////////////////////////////

enum class FontWeight
{
	Normal = 0,
	Bold = 1
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Format
{
	Format(Color color, FontWeight fontWeight = FontWeight::Normal)
	:	m_color{color},
		m_fontWeight{fontWeight}
	{
	}

	Color m_color;
	FontWeight m_fontWeight;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Format &format)
{
	if (!stream.supportsColor())
		return stream;

	const auto fontWeightCode = static_cast<size_t>(format.m_fontWeight);
	const auto colorCode = 30 + static_cast<size_t>(format.m_color);

	return (stream << "\033[" << fontWeightCode << ";" << colorCode << "m");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct ResetFormat
{
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const ResetFormat &)
{
	if (!stream.supportsColor())
		return stream;

	return (stream << "\033[0m");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Token
{
	Token(const std::string &name)
	:	name(name)
	{
	}

	const std::string &name;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

struct RuleName: public Token
{
	RuleName(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const RuleName &keyword)
{
	return (stream
		<< utils::Format(utils::Color::White, utils::FontWeight::Bold)
		<< keyword.name
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Keyword: public Token
{
	Keyword(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Keyword &keyword)
{
	return (stream
		<< utils::Format(utils::Color::Blue, utils::FontWeight::Normal)
		<< keyword.name
		<< utils::ResetFormat());
}
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Number: public Token
{
	Number(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Number &number)
{
	return (stream
		<< utils::Format(utils::Color::Yellow, utils::FontWeight::Normal)
		<< number.name
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Variable: public Token
{
	Variable(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Variable &variable)
{
	return (stream
		<< utils::Format(utils::Color::Green, utils::FontWeight::Bold)
		<< variable.name
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct ASPVariable: public Token
{
	ASPVariable(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const ASPVariable &aspVariable)
{
	if (aspVariable.name.empty())
		return stream;

	// TODO: check that char cast is safe
	return (stream
		<< utils::Format(utils::Color::Green, utils::FontWeight::Bold)
		<< static_cast<char>(std::toupper(aspVariable.name.front()))
		<< aspVariable.name.c_str() + 1
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct String: public Token
{
	String(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const String &string)
{
	return (stream
		<< utils::Format(utils::Color::Green, utils::FontWeight::Normal)
		<< "\"" << string.name << "\""
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Boolean: public Token
{
	Boolean(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Boolean &string)
{
	return (stream
		<< utils::Format(utils::Color::Red, utils::FontWeight::Normal)
		<< string.name
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Reserved: public Token
{
	Reserved(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Reserved &string)
{
	return (stream
		<< utils::Format(utils::Color::White, utils::FontWeight::Normal)
		<< string.name
		<< utils::ResetFormat());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Heading1: public Token
{
	Heading1(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Heading1 &heading1)
{
	return (stream
		<< utils::Format(utils::Color::Blue, utils::FontWeight::Bold)
		<< "%---------------------------------------" << std::endl
		<< "% " << heading1.name << std::endl
		<< "%---------------------------------------"
		<< utils::ResetFormat()
		<< std::endl);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Heading2: public Token
{
	Heading2(const std::string &name)
	:	Token(name)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogStream &operator<<(LogStream &stream, const Heading2 &heading2)
{
	return (stream
		<< utils::Format(utils::Color::Blue, utils::FontWeight::Bold)
		<< "% " << heading2.name
		<< utils::ResetFormat());
}


////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
