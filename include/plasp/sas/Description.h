#ifndef __PLASP__SAS__DESCRIPTION_H
#define __PLASP__SAS__DESCRIPTION_H

#include <experimental/filesystem>
#include <iosfwd>
#include <memory>
#include <vector>


#include <plasp/sas/AxiomRule.h>
#include <plasp/sas/Goal.h>
#include <plasp/sas/InitialState.h>
#include <plasp/sas/MutexGroup.h>
#include <plasp/sas/Operator.h>
#include <plasp/sas/Variable.h>

#include <parsebase/Parser.h>

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
		static Description fromParser(parsebase::Parser<> &&parser);
		static Description fromStream(std::istream &istream);
		static Description fromFile(const std::experimental::filesystem::path &path);

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

		void parseContent(parsebase::Parser<> &parser);

		void parseVersionSection(parsebase::Parser<> &parser) const;
		void parseMetricSection(parsebase::Parser<> &parser);
		void parseVariablesSection(parsebase::Parser<> &parser);
		void parseMutexSection(parsebase::Parser<> &parser);
		void parseInitialStateSection(parsebase::Parser<> &parser);
		void parseGoalSection(parsebase::Parser<> &parser);
		void parseOperatorSection(parsebase::Parser<> &parser);
		void parseAxiomSection(parsebase::Parser<> &parser);

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
