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
		explicit TranslatorASP(const Description &description);

		void translate(std::ostream &ostream) const;

	private:
		void checkSupport() const;

		void translateDomain(std::ostream &ostream) const;
		void translateProblem(std::ostream &ostream) const;

		const Description &m_description;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
