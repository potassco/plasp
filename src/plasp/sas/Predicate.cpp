#include <plasp/sas/Predicate.h>

#include <iostream>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator <<(std::ostream &ostream, const Predicate &predicate)
{
	if (predicate.arguments.empty())
		return (ostream << predicate.name);

	ostream << predicate.name << "(";

	for (size_t i = 0; i < predicate.arguments.size(); i++)
	{
		if (i > 0)
			ostream << ", ";

		ostream << predicate.arguments[i];
	}

	return (ostream << ")");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
