#ifndef __PDDL__DETAIL__PARSING__ACTION_H
#define __PDDL__DETAIL__PARSING__ACTION_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Action
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class ActionParser
{
	public:
		ActionParser(Context &context, ast::Domain &domain);
		ast::ActionPointer parse();

	private:
		void findSections(ast::Action &action);

		void parseParameterSection(ast::Action &action);
		void parsePreconditionSection(ast::Action &action);
		void parseEffectSection(ast::Action &action);

		// For compatibility with old PDDL versions
		void parseVarsSection(ast::Action &action);

		Context &m_context;
		ast::Domain &m_domain;

		tokenize::StreamPosition m_parametersPosition;
		tokenize::StreamPosition m_preconditionPosition;
		tokenize::StreamPosition m_effectPosition;

		// For compatibility with old PDDL versions
		tokenize::StreamPosition m_varsPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
