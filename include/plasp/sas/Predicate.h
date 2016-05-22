#ifndef __SAS__PREDICATE_H
#define __SAS__PREDICATE_H

#include <iosfwd>
#include <string>
#include <vector>

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
		static Predicate fromSAS(std::istream &istream);

		using Arguments = std::vector<std::string>;

	public:
		void printAsSAS(std::ostream &ostream) const;
		void printAsASP(std::ostream &ostream) const;

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
