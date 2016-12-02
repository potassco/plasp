#ifndef __PLASP__SAS__TRANSLATOR_ASP_H
#define __PLASP__SAS__TRANSLATOR_ASP_H

#include <plasp/output/ColorStream.h>
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
		explicit TranslatorASP(const Description &description, output::ColorStream &outputStream);

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
		output::ColorStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
