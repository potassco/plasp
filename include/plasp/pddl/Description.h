#ifndef __PLASP__PDDL__DESCRIPTION_H
#define __PLASP__PDDL__DESCRIPTION_H

#include <plasp/pddl/Domain.h>
#include <plasp/pddl/Problem.h>

#include <tokenize/Tokenizer.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Description
{
	public:
		static Description fromContext(Context &context);
		static Description fromStream(std::istream &istream, Context &context);
		static Description fromFile(const std::string &path, Context &context);
		static Description fromFiles(const std::vector<std::string> &paths, Context &context);

	public:
		Context &context();
		const Context &context() const;

		const Domain &domain() const;

		bool containsProblem() const;
		const Problem &problem() const;

		void normalize();

	private:
		Description(Context &context);

		void parse();
		void findSections();

		void checkConsistency();

		Context &m_context;

		tokenize::Stream::Position m_domainPosition;
		std::unique_ptr<Domain> m_domain;
		tokenize::Stream::Position m_problemPosition;
		std::unique_ptr<Problem> m_problem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
