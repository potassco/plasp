#ifndef __PLASP__PDDL__TRANSLATOR_ASP_H
#define __PLASP__PDDL__TRANSLATOR_ASP_H

#include <plasp/pddl/Description.h>

#include <iosfwd>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class TranslatorASP
{
	public:
		explicit TranslatorASP(Description &description, output::ColorStream &outputStream);

		void translate() const;

	private:
		void translateDomain() const;
		void translateTypes() const;
		void translatePredicates() const;
		void translateDerivedPredicates() const;
		void translateActions() const;

		void translateProblem() const;
		void translateInitialState() const;
		void translateGoal() const;
		void translateConstants(const std::string &heading, const expressions::Constants &constants) const;

		Description &m_description;
		output::ColorStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
