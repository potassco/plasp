#ifndef __PLASP__SAS__VALUE_H
#define __PLASP__SAS__VALUE_H

#include <iosfwd>
#include <string>
#include <vector>

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

		static Value fromSAS(std::istream &istream);
		static const Value &referenceFromSAS(std::istream &istream, const Variable &variable);

	public:
		Value negated() const;

		void printAsSAS(std::ostream &ostream) const;
		void printAsASP(std::ostream &ostream) const;
		void printAsASPPredicate(std::ostream &ostream) const;

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
