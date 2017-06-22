#ifndef __PLASP__SAS__VARIABLE_H
#define __PLASP__SAS__VARIABLE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <tokenize/Tokenizer.h>

#include <colorlog/ColorStream.h>

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
		static Variable fromSAS(tokenize::Tokenizer<> &tokenizer);
		static const Variable &referenceFromSAS(tokenize::Tokenizer<> &tokenizer, const Variables &variables);

	public:
		void printNameAsASPPredicate(colorlog::ColorStream &outputStream) const;

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
