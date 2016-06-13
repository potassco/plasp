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
		explicit TranslatorASP(const Description &description, std::ostream &ostream);

		void translate() const;

	private:
		void checkSupport() const;

		void translateDomain() const;
		void translateTypes() const;
		void translateConstants() const;
		void translatePredicates() const;
		void translateActions() const;

		void translateProblem() const;
		void translateObjects() const;

		void translateVariablesHead(const expressions::Variables &variables) const;
  		void translateVariablesBody(const expressions::Variables &variables) const;
		void translatePredicate(const expressions::Predicate &predicate) const;

		const Description &m_description;
		std::ostream &m_ostream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
