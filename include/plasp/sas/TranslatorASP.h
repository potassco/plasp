#ifndef __PLASP__SAS__TRANSLATOR_ASP_H
#define __PLASP__SAS__TRANSLATOR_ASP_H

#include <plasp/sas/Description.h>
#include <plasp/utils/LogStream.h>

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
		explicit TranslatorASP(const Description &description, utils::LogStream &outputStream);

		void translate() const;

	private:
		void translateRequirements() const;
		void translateInitialState() const;
		void translateGoal() const;
		void translateVariables() const;
		void translateActions() const;
		void translateMutexes() const;
		void translateAxiomRules() const;

		const Description &m_description;
		utils::LogStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
