#ifndef __PLASP__SAS__PREDICATE_H
#define __PLASP__SAS__PREDICATE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <plasp/output/ColorStream.h>

#include <parsebase/Parser.h>

namespace plasp
{
namespace sas
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Predicate
{
	public:
		static Predicate fromSAS(parsebase::Parser<> &parser);

		using Arguments = std::vector<std::string>;

	public:
		void printAsSAS(output::ColorStream &stream) const;
		void printAsASP(output::ColorStream &stream) const;

		const std::string &name() const;
		const Arguments &arguments() const;

	private:
		std::string m_name;
		std::vector<std::string> m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
