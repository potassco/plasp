#ifndef __UTILS__PARSING_H
#define __UTILS__PARSING_H

#include <algorithm>
#include <exception>
#include <iosfwd>
#include <sstream>
#include <string>
#include <typeinfo>

#include <boost/algorithm/string/replace.hpp>

#include <plasp/utils/ParserException.h>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Parsing
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
T parse(std::istream &istream)
{
	T value;

	try
	{
		istream >> value;
	}
	catch (const std::exception &e)
	{
		throw ParserException(std::string("Could not parse value of type ") + typeid(T).name() + " (" + e.what() + ")");
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
void parseExpected(std::istream &istream, const T &expectedValue)
{
	const auto value = parse<T>(istream);

	if (value == expectedValue)
		return;

	std::stringstream errorStream;

	errorStream << "Invalid format, expected " << expectedValue << ", got " + value;

	throw utils::ParserException(errorStream.str());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::string escapeASP(const std::string &string)
{
	auto escaped = string;

	boost::replace_all(escaped, "_", "__");
	boost::replace_all(escaped, "-", "_h");

	return escaped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::string unescapeASP(const std::string &string)
{
	auto unescaped = string;

	boost::replace_all(unescaped, "_h", "-");
	boost::replace_all(unescaped, "__", "_");

	return unescaped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
