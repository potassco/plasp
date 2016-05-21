#ifndef __UTILS__PARSING_H
#define __UTILS__PARSING_H

#include <exception>
#include <iosfwd>
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

}
}

#endif
