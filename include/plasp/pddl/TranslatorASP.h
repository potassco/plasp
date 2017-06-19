#ifndef __PLASP__PDDL__TRANSLATOR_ASP_H
#define __PLASP__PDDL__TRANSLATOR_ASP_H

#include <plasp/output/ColorStream.h>

#include <pddlparse/ASTForward.h>
#include <pddlparse/Parse.h>

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
		explicit TranslatorASP(const ::pddl::ast::Description &description, output::ColorStream &outputStream);

		void translate() const;

	private:
		void translateDomain() const;
		void translateTypes() const;
		void translatePredicates() const;
		void translateActions() const;

		void translateProblem() const;
		void translateInitialState() const;
		void translateGoal() const;
		void translateConstants(const std::string &heading, const ::pddl::ast::ConstantDeclarations &constants) const;

		const ::pddl::ast::Description &m_description;
		output::ColorStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
