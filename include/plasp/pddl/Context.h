#ifndef __PLASP__PDDL__CONTEXT_H
#define __PLASP__PDDL__CONTEXT_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <plasp/pddl/Predicate.h>
#include <plasp/pddl/Type.h>

namespace plasp
{
namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Context
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Context
{
	public:
		std::vector<std::unique_ptr<PrimitiveType>> primitiveTypes;
		std::unordered_map<std::string, PrimitiveType *> primitiveTypesHashMap;

		std::vector<std::unique_ptr<EitherType>> eitherTypes;

		std::vector<std::unique_ptr<Predicate>> predicates;
		std::unordered_map<PredicateHashMapKey, Predicate *> predicatesHashMap;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
