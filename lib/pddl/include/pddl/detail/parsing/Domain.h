#ifndef __PDDL__DETAIL__PARSING__DOMAIN_H
#define __PDDL__DETAIL__PARSING__DOMAIN_H

#include <pddl/AST.h>
#include <pddl/Context.h>

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

		tokenize::StreamPosition m_requirementsPosition;
		tokenize::StreamPosition m_typesPosition;
		tokenize::StreamPosition m_constantsPosition;
		tokenize::StreamPosition m_predicatesPosition;
		std::vector<tokenize::StreamPosition> m_actionPositions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
