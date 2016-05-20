#ifndef __SAS__DESCRIPTION_H
#define __SAS__DESCRIPTION_H

#include <iosfwd>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <plasp/sas/MutexGroup.h>
#include <plasp/sas/Operator.h>
#include <plasp/sas/Variable.h>
#include <plasp/sas/AxiomRule.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Description
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Description
{
	public:
		static Description fromStream(std::istream &istream);
		static Description fromFile(const boost::filesystem::path &path);

	public:
		Description();

		const std::vector<Variable> &variables() const;
		const std::vector<MutexGroup> &mutexGroups() const;
		const std::vector<AssignedVariable> &initialStateFacts() const;
		const std::vector<AssignedVariable> &goalFacts() const;
		const std::vector<Operator> &operators() const;
		const std::vector<AxiomRule> &axiomRules() const;

		void print(std::ostream &ostream) const;

	private:
		void parseSectionIdentifier(std::istream &istream, const std::string &expectedSectionIdentifier) const;

		template<class T>
		T parse(std::istream &istream) const;
		const Variable &parseVariable(std::istream &istream) const;
		const Value &parseValue(std::istream &istream, const Variable &variable) const;
		AssignedVariable parseAssignedVariable(std::istream &istream) const;
		VariableTransition parseVariableTransition(std::istream &istream) const;

		void parseVersionSection(std::istream &istream) const;
		void parseMetricSection(std::istream &istream);
		void parseVariablesSection(std::istream &istream);
		void parseMutexSection(std::istream &istream);
		void parseInitialStateSection(std::istream &istream);
		void parseGoalSection(std::istream &istream);
		void parseOperatorSection(std::istream &istream);
		void parseAxiomSection(std::istream &istream);

		bool m_usesActionCosts;

		std::vector<Variable> m_variables;
		std::vector<MutexGroup> m_mutexGroups;
		std::vector<AssignedVariable> m_initialStateFacts;
		std::vector<AssignedVariable> m_goalFacts;
		std::vector<Operator> m_operators;
		std::vector<AxiomRule> m_axiomRules;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
