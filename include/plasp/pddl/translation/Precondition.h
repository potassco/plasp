#ifndef __PLASP__PDDL__TRANSLATION__PRECONDITION_H
#define __PLASP__PDDL__TRANSLATION__PRECONDITION_H

#include <plasp/output/Formatting.h>
#include <plasp/pddl/Description.h>
#include <plasp/pddl/translation/Primitives.h>

namespace plasp
{
namespace pddl
{
namespace translation
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Precondition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class PrintObjectName>
void translatePreconditionDisjunction(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, const std::vector<Expressions> &preconditionDisjunction);
template<class PrintObjectName>
void translatePreconditionConjunction(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, size_t disjunctionID, const Expressions &preconditionConjunction);
template<class PrintObjectName>
void translatePrecondition(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, size_t disjunctionID, const Expression &precondition);

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class PrintObjectName>
inline void translatePreconditionDisjunction(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, const std::vector<Expressions> &preconditionDisjunction)
{
	for (size_t i = 0; i < preconditionDisjunction.size(); i++)
		translatePreconditionConjunction(outputStream, objectType, printObjectName, i, preconditionDisjunction[i]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class PrintObjectName>
inline void translatePreconditionConjunction(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, size_t disjunctionID, const Expressions &preconditionConjunction)
{
	for (size_t i = 0; i < preconditionConjunction.size(); i++)
		translatePrecondition(outputStream, objectType, printObjectName, disjunctionID, *preconditionConjunction[i]);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class PrintObjectName>
inline void translatePrecondition(output::ColorStream &outputStream, const std::string &objectType, PrintObjectName printObjectName, size_t disjunctionID, const Expression &precondition)
{
	outputStream << std::endl << output::Function("precondition") << "(";

	printObjectName(outputStream);

	outputStream
		<< ", " << output::Keyword("disjunct")
		<< "(" << output::Number<decltype(disjunctionID)>(disjunctionID)
		<< "), ";

	translateLiteral(outputStream, precondition);

	outputStream << ") :- " << output::Function(objectType.c_str()) << "(";

	printObjectName(outputStream);

	outputStream << ").";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
