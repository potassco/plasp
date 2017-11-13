#ifndef __PLASP__SAS__PREDICATE_H
#define __PLASP__SAS__PREDICATE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <tokenize/Tokenizer.h>

#include <colorlog/ColorStream.h>

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
		static Predicate fromSAS(tokenize::Tokenizer<> &tokenizer);

		using Arguments = std::vector<std::string>;

	public:
		void printAsSAS(colorlog::ColorStream &stream) const;
		void printAsASP(colorlog::ColorStream &stream) const;

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
