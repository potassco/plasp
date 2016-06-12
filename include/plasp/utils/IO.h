#ifndef __PLASP__UTILS__IO_H
#define __PLASP__UTILS__IO_H

#include <boost/algorithm/string/replace.hpp>

namespace plasp
{
namespace utils
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IO
//
////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::string escapeASP(const std::string &string)
{
	auto escaped = string;

	boost::replace_all(escaped, "_", "__");
	boost::replace_all(escaped, "-", "_h");
	boost::replace_all(escaped, "@", "_a");

	return escaped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::string unescapeASP(const std::string &string)
{
	auto unescaped = string;

	boost::replace_all(unescaped, "_a", "@");
	boost::replace_all(unescaped, "_h", "-");
	boost::replace_all(unescaped, "__", "_");

	return unescaped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::string escapeASPVariable(const std::string &string)
{
	auto escaped = escapeASP(string);

	escaped.front() = std::toupper(escaped.front());

	return escaped;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
