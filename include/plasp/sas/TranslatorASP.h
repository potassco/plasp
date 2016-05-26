#ifndef __PLASP__SAS__TRANSLATOR_ASP_H
#define __PLASP__SAS__TRANSLATOR_ASP_H

#include <plasp/sas/Description.h>

#include <iosfwd>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TranslatorASP
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class TranslatorASP
{
	public:
		explicit TranslatorASP(const Description &description);

		void translate(std::ostream &ostream) const;

	private:
		void translateRequirements(std::ostream &ostream) const;
		void translateInitialState(std::ostream &ostream) const;
		void translateGoal(std::ostream &ostream) const;
		void translateVariables(std::ostream &ostream) const;
		void translateActions(std::ostream &ostream) const;
		void translateMutexes(std::ostream &ostream) const;
		void translateAxiomRules(std::ostream &ostream) const;

		const Description &m_description;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
