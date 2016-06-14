#ifndef __PLASP__SAS__VALUE_H
#define __PLASP__SAS__VALUE_H

#include <iosfwd>
#include <string>
#include <vector>

#include <plasp/utils/LogStream.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace sas
{

// Forward declarations
class Variable;

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Value
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Value;
using Values = std::vector<Value>;

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Value
{
	public:
		enum class Sign
		{
			Positive,
			Negative
		};

		static const Value Any;
		static const Value None;

		static Value fromSAS(utils::Parser &parser);
		static const Value &referenceFromSAS(utils::Parser &parser, const Variable &variable);

	public:
		Value negated() const;

		void printAsSAS(utils::LogStream &outputStream) const;
		void printAsASP(utils::LogStream &outputStream) const;
		void printAsASPPredicate(utils::LogStream &outputStream) const;

		Sign sign() const;
		const std::string &name() const;

	private:
		static const Value reserved(const std::string &name);

	private:
		Value();

		Sign m_sign;
		std::string m_name;
		bool m_hasArguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

bool operator ==(const Value &value1, const Value &value2);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
