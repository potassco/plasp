#ifndef __UTILS__PARSING_H
#define __UTILS__PARSING_H

#include <exception>
#include <iosfwd>
#include <sstream>
#include <string>
#include <typeinfo>

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

}
}

#endif
