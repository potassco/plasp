#ifndef __PLASP__SAS__TRANSLATOR_ASP_H
#define __PLASP__SAS__TRANSLATOR_ASP_H

#include <iosfwd>

#include <colorlog/ColorStream.h>

#include <plasp/sas/Description.h>

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
		explicit TranslatorASP(const Description &description, colorlog::ColorStream &outputStream);

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
		colorlog::ColorStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
