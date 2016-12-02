#ifndef __PLASP__SAS__DESCRIPTION_H
#define __PLASP__SAS__DESCRIPTION_H

#include <iosfwd>
#include <memory>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <plasp/input/Parser.h>
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
		static Description fromParser(input::Parser<> &&parser);
		static Description fromStream(std::istream &istream);
		static Description fromFile(const boost::filesystem::path &path);

	public:
		bool usesActionCosts() const;
		const Variables &variables() const;
		const MutexGroups &mutexGroups() const;
		const InitialState &initialState() const;
		const Goal &goal() const;
		const Operators &operators() const;
		const AxiomRules &axiomRules() const;

		bool usesAxiomRules() const;
		bool usesConditionalEffects() const;

		bool hasRequirements() const;

	private:
		Description();

		void parseContent(input::Parser<> &parser);

		void parseVersionSection(input::Parser<> &parser) const;
		void parseMetricSection(input::Parser<> &parser);
		void parseVariablesSection(input::Parser<> &parser);
		void parseMutexSection(input::Parser<> &parser);
		void parseInitialStateSection(input::Parser<> &parser);
		void parseGoalSection(input::Parser<> &parser);
		void parseOperatorSection(input::Parser<> &parser);
		void parseAxiomSection(input::Parser<> &parser);

		bool m_usesActionCosts;

		Variables m_variables;
		MutexGroups m_mutexGroups;
		std::unique_ptr<InitialState> m_initialState;
		std::unique_ptr<Goal> m_goal;
		Operators m_operators;
		AxiomRules m_axiomRules;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
