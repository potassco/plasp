#ifndef __PDDL_PARSE__DETAIL__PARSING__ACTION_H
#define __PDDL_PARSE__DETAIL__PARSING__ACTION_H

#include <pddlparse/ASTForward.h>
#include <pddlparse/Context.h>

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

		tokenize::Stream::Position m_parametersPosition;
		tokenize::Stream::Position m_preconditionPosition;
		tokenize::Stream::Position m_effectPosition;

		// For compatibility with old PDDL versions
		tokenize::Stream::Position m_varsPosition;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
