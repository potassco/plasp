#ifndef __PLASP__SAS__PREDICATE_H
#define __PLASP__SAS__PREDICATE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <plasp/utils/LogStream.h>
#include <plasp/utils/Parser.h>

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
		static Predicate fromSAS(utils::Parser<> &parser);

		using Arguments = std::vector<std::string>;

	public:
		void printAsSAS(utils::LogStream &outputStream) const;
		void printAsASP(utils::LogStream &outputStream) const;

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
