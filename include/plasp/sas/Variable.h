#ifndef __SAS__VARIABLE_H
#define __SAS__VARIABLE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <plasp/sas/Value.h>

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
		static Variable fromSAS(std::istream &istream);
		static const Variable &referenceFromSAS(std::istream &istream, const Variables &variables);

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
