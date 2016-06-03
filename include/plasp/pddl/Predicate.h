#ifndef __PLASP__PDDL__PREDICATE_H
#define __PLASP__PDDL__PREDICATE_H

#include <boost/functional/hash.hpp>

#include <plasp/pddl/expressions/Variable.h>
#include <plasp/utils/Parser.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Predicate
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context;

struct PredicateHashMapKey
{
	std::string name;
	size_t arity;

	bool operator==(const PredicateHashMapKey &other) const
	{
		return arity == other.arity && name == other.name;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Predicate
{
	public:
		static Predicate &parseDeclaration(utils::Parser &parser, Context &context);

	public:
		const std::string &name() const;
		const expressions::Variables &arguments() const;

		bool isDeclared() const;

	private:
		Predicate(std::string name);

		void setDeclared();

		bool m_isDeclared;

		std::string m_name;
		expressions::Variables m_arguments;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace std
{
	template<>
	struct hash<plasp::pddl::PredicateHashMapKey>
	{
		std::size_t operator()(const plasp::pddl::PredicateHashMapKey &key) const
		{
			std::size_t seed = 0;

			boost::hash_combine(seed, key.name);
			boost::hash_combine(seed, key.arity);

			return seed;
		}
	};
}

#endif
