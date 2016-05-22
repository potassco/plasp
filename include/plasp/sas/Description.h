#ifndef __SAS__DESCRIPTION_H
#define __SAS__DESCRIPTION_H

#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <plasp/sas/AxiomRule.h>
#include <plasp/sas/Goal.h>
#include <plasp/sas/InitialState.h>
#include <plasp/sas/MutexGroup.h>
#include <plasp/sas/Operator.h>
#include <plasp/sas/Variable.h>

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
		bool usesActionCosts() const;
		const std::vector<Variable> &variables() const;
		const std::vector<MutexGroup> &mutexGroups() const;
		const InitialState &initialState() const;
		const Goal &goal() const;
		const std::vector<Operator> &operators() const;
		const std::vector<AxiomRule> &axiomRules() const;

		void print(std::ostream &ostream) const;

	private:
		Description();

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
		std::unique_ptr<InitialState> m_initialState;
		std::unique_ptr<Goal> m_goal;
		std::vector<Operator> m_operators;
		std::vector<AxiomRule> m_axiomRules;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
