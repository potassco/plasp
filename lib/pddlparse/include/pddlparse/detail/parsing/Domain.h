#ifndef __PDDL_PARSE__DETAIL__PARSING__DOMAIN_H
#define __PDDL_PARSE__DETAIL__PARSING__DOMAIN_H

#include <experimental/optional>

#include <pddlparse/AST.h>
#include <pddlparse/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Domain
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class DomainParser
{
	public:
		DomainParser(Context &context);
		ast::DomainPointer parse();

	private:
		void findSections(ast::Domain &domain);

		void parseRequirementSection(ast::Domain &domain);
		void computeDerivedRequirements(ast::Domain &domain);
		void parseTypeSection(ast::Domain &domain);
		void parseConstantSection(ast::Domain &domain);
		void parsePredicateSection(ast::Domain &domain);
		void parseActionSection(ast::Domain &domain);

		Context &m_context;

		tokenize::Stream::Position m_requirementsPosition;
		tokenize::Stream::Position m_typesPosition;
		tokenize::Stream::Position m_constantsPosition;
		tokenize::Stream::Position m_predicatesPosition;
		std::vector<tokenize::Stream::Position> m_actionPositions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
