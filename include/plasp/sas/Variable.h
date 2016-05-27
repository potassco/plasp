#ifndef __PLASP__SAS__VARIABLE_H
#define __PLASP__SAS__VARIABLE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <plasp/sas/Value.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variable
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Variable;
using Variables = std::vector<Variable>;

////////////////////////////////////////////////////////////////////////////////////////////////////

class Variable
{
	public:
		static Variable fromSAS(utils::Parser &parser);
		static const Variable &referenceFromSAS(utils::Parser &parser, const Variables &variables);

	public:
		void printNameAsASPPredicate(std::ostream &ostream) const;

		const std::string &name() const;
		int axiomLayer() const;
		const Values &values() const;

	private:
		Variable();

		std::string m_name;
		int m_axiomLayer;
		Values m_values;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
