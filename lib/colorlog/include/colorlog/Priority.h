#ifndef __COLOR_LOG__PRIORITY_H
#define __COLOR_LOG__PRIORITY_H

#include <cstring>
#include <exception>

namespace colorlog
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Priority
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class Priority
{
	Debug,
	Info,
	Warning,
	Error
};

////////////////////////////////////////////////////////////////////////////////////////////////////

inline constexpr const char *priorityName(Priority priority)
{
	switch (priority)
	{
		case Priority::Debug:
			return "debug";
		case Priority::Info:
			return "info";
		case Priority::Warning:
			return "warning";
		case Priority::Error:
			return "error";
	}

	return "unknown";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

inline Priority priorityFromName(const char *priorityName)
{
	if (std::strcmp(priorityName, "debug") == 0)
		return Priority::Debug;
	if (std::strcmp(priorityName, "info") == 0)
		return Priority::Info;
	if (std::strcmp(priorityName, "warning") == 0)
		return Priority::Warning;
	if (std::strcmp(priorityName, "error") == 0)
		return Priority::Error;

	throw std::runtime_error("unknown log priority");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
