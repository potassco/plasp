#ifndef __PLASP__PDDL__TRANSLATOR_ASP_H
#define __PLASP__PDDL__TRANSLATOR_ASP_H

#include <colorlog/ColorStream.h>

#include <pddl/NormalizedASTForward.h>
#include <pddl/Parse.h>

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
		explicit TranslatorASP(const ::pddl::normalizedAST::Description &description, colorlog::ColorStream &outputStream);

		void translate() const;

	private:
		void translateDomain() const;
		void translateUtils() const;
		void translateTypes() const;
		void translatePredicates() const;
		void translateDerivedPredicates(const ::pddl::normalizedAST::DerivedPredicateDeclarations &derivedPredicates) const;
		void translateActions() const;

		void translateProblem() const;
		void translateInitialState() const;
		void translateGoal() const;
		void translateConstants(const std::string &heading, const ::pddl::ast::ConstantDeclarations &constants) const;

		const ::pddl::normalizedAST::Description &m_description;
		colorlog::ColorStream &m_outputStream;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
